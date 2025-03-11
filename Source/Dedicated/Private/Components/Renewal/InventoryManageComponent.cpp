// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Renewal/InventoryManageComponent.h"

#include "BaseDebugHelper.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PlayerAttributeSet.h"
#include "Characters/PubgCharacter.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"
#include "Net/UnrealNetwork.h"
#include "Subsystem/PubgDataTableManageSubsystem.h"
#include "Weapon/WeaponBase.h"


bool FInventoryList::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaSerializeInfo)
{
	return FastArrayDeltaSerialize<FInventoryListItem, FInventoryList>(Items, DeltaSerializeInfo, *this);
}

void FInventoryList::AddItem(int32 InItemID, int32 InItemAmount)
{
	FInventoryListItem& NewItem = Items.AddDefaulted_GetRef();
	NewItem.ItemID = InItemID;
	NewItem.ItemAmount = InItemAmount;
	MarkItemDirty(NewItem);
}

void FInventoryList::UpdateItem(int32 InItemID, int32 InItemDeltaAmount)
{
	for (auto& Item : Items)
	{
		if (Item.ItemID == InItemID)
		{
			Item.ItemAmount += InItemDeltaAmount;
			MarkItemDirty(Item);
			return;
		}
	}
	
	// 아이템이 없으므로 add
	AddItem(InItemID, InItemDeltaAmount);
}

bool FInventoryList::HasItem(int32 InItemID, int32 InItemAmount) const
{
	for (auto& Item : Items)
	{
		if (Item.ItemID == InItemID)
			return Item.ItemAmount >= InItemAmount;
	}
	return false;
}

int32 FInventoryList::GetItemAmount(int32 InItemID) const
{
	for (auto& Item : Items)
	{
		if (Item.ItemID == InItemID)
			return Item.ItemAmount;
	}
	return 0;
}

// Sets default values for this component's properties
UInventoryManageComponent::UInventoryManageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	SetIsReplicatedByDefault(true);
	// ...
}

void UInventoryManageComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UInventoryManageComponent, InventoryList, COND_AutonomousOnly);
	DOREPLIFETIME_CONDITION(UInventoryManageComponent, CurrentWeight, COND_AutonomousOnly);
}

int32 UInventoryManageComponent::CalculateAvailableAmount(int32 InItemID, int32 InItemAmount)
{
	const float MaxWeight = GetMaxWeight();
	int32 AvailableAmount = InItemAmount;
	if (const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(InItemID))
	{
		const float ItemWeight = FMath::IsNearlyEqual(ItemData->ItemWeight, 0) ? 0.0001f : ItemData->ItemWeight;
		if (InItemAmount * ItemWeight > MaxWeight)
		{
			AvailableAmount = FMath::Floor((MaxWeight - CurrentWeight) / ItemWeight);
		}
	}
	else
	{
		Debug::PrintError("There is no Item Subsystem");
	}
	return AvailableAmount;
}

void UInventoryManageComponent::UpdateWeight(int32 InItemID, int32 InItemAmount)
{
	if (const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(InItemID))
	{
		const float ItemWeight = ItemData->ItemWeight;
		CurrentWeight += InItemAmount * ItemWeight;
	}
	else
	{
		Debug::PrintError("There is no Item Subsystem");
	}
}

float UInventoryManageComponent::GetMaxWeight() const
{
	UBaseAbilitySystemComponent* ASC = UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(GetOwner());
	float MaxWeight = 0.0f;
	if (ASC)
	{
		MaxWeight = ASC->GetNumericAttribute(UPlayerAttributeSet::GetInventoryCapacityAttribute());
	}
	else
	{
		Debug::PrintError("Owner don't have a Ability system component");
	}
	return MaxWeight;
}

bool UInventoryManageComponent::AcquireItem(int32 InItemID, int32 InAmount, int32& OutRemainAmount)
{
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		const int32 ActualAcquiredAmount = CalculateAvailableAmount(InItemID, InAmount);
		if (ActualAcquiredAmount > 0)
		{
			InventoryList.UpdateItem(InItemID, ActualAcquiredAmount);
			UpdateWeight(InItemID, ActualAcquiredAmount);
			OutRemainAmount = InAmount - ActualAcquiredAmount;
			return true;
		}
	}
	return false;
}

int32 UInventoryManageComponent::DiscardItem(int32 InItemID, int32 InAmount)
{
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		const int32 CurrentAmount = InventoryList.GetItemAmount(InItemID);
		if (CurrentAmount > 0)
		{
			const int32 DiscardAmount = FMath::Min(CurrentAmount, InAmount);
			InventoryList.UpdateItem(InItemID, -DiscardAmount);
			UpdateWeight(InItemID, -DiscardAmount);
			return DiscardAmount;
		}
	}
	return 0;
}

void UInventoryManageComponent::ClearItems()
{
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		InventoryList.GetItemsRef().Empty();
		InventoryList.MarkArrayDirty();
	}
}

void UInventoryManageComponent::UseItem(int32 InItemID, int32 InAmount)
{
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		const int32 CurrentAmount = InventoryList.GetItemAmount(InItemID);
		if (CurrentAmount < InAmount) return;
		InventoryList.UpdateItem(InItemID, -InAmount);
		UpdateWeight(InItemID, -InAmount);
	}
}

void UInventoryManageComponent::Server_UseItem_Implementation(int32 InItemID, int32 InAmount)
{
	UseItem(InItemID, InAmount);
}

void UInventoryManageComponent::Server_EquipParts_Implementation(int32 InPartsItemID,
	const FGameplayTag& InWeaponSlotTag)
{
	if (InPartsItemID <= 0 || InWeaponSlotTag.IsValid() == false) return;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		if (UEquipManageComponent* EquipManager = GetOwner()->FindComponentByClass<UEquipManageComponent>())
		{
			if (AWeaponBase* TargetWeapon = EquipManager->GetWeaponInSlot(InWeaponSlotTag); IsValid(TargetWeapon))
			{
				TargetWeapon->EquipWeaponParts(InPartsItemID);
				UseItem(InPartsItemID, 1);
			}
		}
	}
}

bool UInventoryManageComponent::HasItem(int32 InItemID, int32 InAmount) const
{
	return InventoryList.HasItem(InItemID, InAmount);
}

int32 UInventoryManageComponent::GetItemAmount(int32 InItemID) const
{
	return InventoryList.GetItemAmount(InItemID);
}

void UInventoryManageComponent::OnRep_UpdateItem()
{

	// Update Ammo
	if (UEquipManageComponent* EquipManager = GetOwner()->FindComponentByClass<UEquipManageComponent>())
	{
		EquipManager->OnRep_CurrentWeaponSlot();
	}

	// Update UI
	if (APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(GetOwner()))
	{
		PlayerCharacter->UpdateInventory();
	}
}

