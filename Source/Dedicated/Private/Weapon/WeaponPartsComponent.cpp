// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponPartsComponent.h"

#include "BaseDebugHelper.h"
#include "BaseGameplayTags.h"
#include "Characters/PubgCharacter.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "HUD/HUDBase.h"
#include "HUD/UI/InventoryMainWidget.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"
#include "Net/UnrealNetwork.h"
#include "Subsystem/PubgDataTableManageSubsystem.h"
#include "Weapon/Parts/WeaponPartsBase.h"
#include "Weapon/Parts/Sight/WeaponSightBase.h"


void FWeaponPartsInfo::SetPartsInSlot(const FGameplayTag& InSlotTag, AWeaponPartsBase* InWeaponParts)
{
	if (InSlotTag.IsValid() == false) return;
	if (InSlotTag.MatchesTag(BaseGameplayTags::Item_Parts_Magazine))
	{
		MagazineParts = InWeaponParts;
	}
	else if (InSlotTag.MatchesTag(BaseGameplayTags::Item_Parts_Sight))
	{
		SightParts = InWeaponParts;
	}
	else if (InSlotTag.MatchesTag(BaseGameplayTags::Item_Parts_Grip))
	{
		GripParts = InWeaponParts;
	}
	else if (InSlotTag.MatchesTag(BaseGameplayTags::Item_Parts_Muzzle))
	{
		MuzzleParts = InWeaponParts;
	}
	else if (InSlotTag.MatchesTag(BaseGameplayTags::Item_Parts_Stock))
	{
		StockParts = InWeaponParts;
	}
}

AWeaponPartsBase* FWeaponPartsInfo::GetPartsInSlot(const FGameplayTag& InSlotTag)
{
	if (InSlotTag.IsValid() == false) return nullptr;
	if (InSlotTag.MatchesTag(BaseGameplayTags::Item_Parts_Magazine))
	{
		return MagazineParts;
	}
	else if (InSlotTag.MatchesTag(BaseGameplayTags::Item_Parts_Sight))
	{
		return SightParts;
	}
	else if (InSlotTag.MatchesTag(BaseGameplayTags::Item_Parts_Grip))
	{
		return GripParts;
	}
	else if (InSlotTag.MatchesTag(BaseGameplayTags::Item_Parts_Muzzle))
	{
		return MuzzleParts;
	}
	else if (InSlotTag.MatchesTag(BaseGameplayTags::Item_Parts_Stock))
	{
		return StockParts;
	}

	return nullptr;
}

// Sets default values for this component's properties
UWeaponPartsComponent::UWeaponPartsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	// ...
}

void UWeaponPartsComponent::BeginPlay()
{
	Super::BeginPlay();
	if (AvailablePartsSlot.IsEmpty())
	{
		SetActive(false);
		SetIsReplicated(false);
	}

	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		for (auto& PartsID : DefaultPartsIDSetting)
		{
			EquipDefaultParts(PartsID);
		}
	}
}

void UWeaponPartsComponent::AttachPartsInSlot(const FGameplayTag& InSlotTag, AWeaponPartsBase* InWeaponParts)
{
	if (IsValid(InWeaponParts) == false || InSlotTag.IsValid() == false) return;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		if (USkeletalMeshComponent* WeaponMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
		{
			static const TMap<FGameplayTag, FName> PartsBoneMap =
				{
				{BaseGameplayTags::Item_Parts_Magazine, FName("Gun_magazinesocket")},
				{BaseGameplayTags::Item_Parts_Sight, FName("Gun_sightSocket")},
				{BaseGameplayTags::Item_Parts_Sight_Rifle, FName("Gun_sightSocket")},
				{BaseGameplayTags::Item_Parts_Grip, FName("Gun_gripSocket")},
				{BaseGameplayTags::Item_Parts_Muzzle, FName("Gun_muzzleSocket")},
				{BaseGameplayTags::Item_Parts_Muzzle_Suppressor_AR, FName("Gun_muzzleSocket")},
				{BaseGameplayTags::Item_Parts_Muzzle_Suppressor_GROZA, FName("Gun_muzzleSocket")},
				{BaseGameplayTags::Item_Parts_Muzzle_Suppressor_SR, FName("Gun_muzzleSocket")},
				{BaseGameplayTags::Item_Parts_Muzzle_Suppressor_SMG, FName("Gun_muzzleSocket")},
				{BaseGameplayTags::Item_Parts_Stock, FName("Gun_stockSocket")}
				};
			if (PartsBoneMap.Contains(InSlotTag))
			{
				const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
				InWeaponParts->AttachToComponent(WeaponMesh, AttachmentRules, PartsBoneMap[InSlotTag]);
				//Debug::Print(PartsBoneMap[InSlotTag].ToString());
				// TODO: if offset needs, add code here.
			}
		}
	}
}

void UWeaponPartsComponent::DetachParts(AWeaponPartsBase* InWeaponParts)
{
	if (IsValid(InWeaponParts) == false) return;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		const FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
		InWeaponParts->DetachFromActor(DetachmentRules);
	}
}

void UWeaponPartsComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// DOREPLIFETIME_CONDITION(UWeaponPartsComponent, CurrentWeaponPartsInfo, COND_InitialOrOwner);
	DOREPLIFETIME(UWeaponPartsComponent, CurrentWeaponPartsInfo);
	DOREPLIFETIME(UWeaponPartsComponent, DefaultWeaponPartsInfo);
}

void UWeaponPartsComponent::EquipDefaultParts(int32 InItemID)
{
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		if (const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(InItemID))
		{
			if (ItemData && ItemData->ItemActorClass.IsNull() == false)
			{
				if (UClass* ItemClass = ItemData->ItemActorClass.LoadSynchronous())
				{
					if (AWeaponPartsBase* WeaponParts = GetWorld()->SpawnActor<AWeaponPartsBase>(ItemClass))
					{
						DefaultWeaponPartsInfo.SetPartsInSlot(WeaponParts->GetEquipSlotTag(), WeaponParts);
						WeaponParts->OnEquipParts(GetOwner());
						WeaponParts->SetOwner(GetOwner());
						AttachPartsInSlot(WeaponParts->GetEquipSlotTag(), WeaponParts);
					}
				}
			}
		}
	}
}

bool UWeaponPartsComponent::EquipParts(int32 InItemID)
{
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		if (UPubgDataTableManageSubsystem* DB = UPubgDataTableManageSubsystem::GetInstance())
		{
			const FInventoryDataStruct* ItemData = DB->FindRow<FInventoryDataStruct>(FName(FString::FromInt(InItemID)));
			if (ItemData && ItemData->ItemActorClass.IsNull() == false)
			{
				if (UClass* ItemClass = ItemData->ItemActorClass.LoadSynchronous())
				{
					const AWeaponPartsBase* CDO = GetMutableDefault<AWeaponPartsBase>(ItemClass);
					if (IsValid(CurrentWeaponPartsInfo.GetPartsInSlot(CDO->GetEquipSlotTag())))
					{
						Debug::PrintError("The Parts is already in slot");
					}
					else if (AWeaponPartsBase* WeaponParts = GetWorld()->SpawnActor<AWeaponPartsBase>(ItemClass))
					{
						CurrentWeaponPartsInfo.SetPartsInSlot(WeaponParts->GetEquipSlotTag(), WeaponParts);
						AWeaponPartsBase* DefaultParts = DefaultWeaponPartsInfo.GetPartsInSlot(WeaponParts->GetEquipSlotTag());
						if (IsValid(DefaultParts))
						{
							DefaultParts->SetActorHiddenInGame(true);
							DefaultParts->OnUnequipParts(GetOwner());
						}
						WeaponParts->OnEquipParts(GetOwner());
						WeaponParts->SetOwner(GetOwner());
						AttachPartsInSlot(WeaponParts->GetEquipSlotTag(), WeaponParts);
						return true;
					}
				}
			}
		}
	}

	return false;
}

int32 UWeaponPartsComponent::UnequipParts(const FGameplayTag& InSlotTag)
{
	int32 PartsItemID = 0;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		AWeaponPartsBase* UnEquippedParts = CurrentWeaponPartsInfo.GetPartsInSlot(InSlotTag);
		if (IsValid(UnEquippedParts))
		{
			UnEquippedParts->OnUnequipParts(GetOwner());
			CurrentWeaponPartsInfo.SetPartsInSlot(InSlotTag, nullptr);
			DetachParts(UnEquippedParts);
			AWeaponPartsBase* DefaultParts = DefaultWeaponPartsInfo.GetPartsInSlot(InSlotTag);
			if (IsValid(DefaultParts))
			{
				DefaultParts->SetActorHiddenInGame(false);
				DefaultParts->OnEquipParts(GetOwner());
			}
			PartsItemID = UnEquippedParts->GetItemID(); 
			UnEquippedParts->Destroy();
		}
	}
	return PartsItemID;
}

void UWeaponPartsComponent::OnRep_UpdateCurrentParts()
{
	if (APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(GetOwner()->GetOwner()))
	{
		if (PlayerCharacter->GetHUDBase())
		{
			PlayerCharacter->GetHUDBase()->GetInventoryMainWidget()->UpdateWeaponParts();
			Debug::Print("UWeaponPartsComponent OnRep :: Call InventoryUI");
		}
		else
		{
			Debug::Print("UWeaponPartsComponent OnRep :: Cast Failed GetHUD");
		}
	}
	else
	{
		Debug::Print("UWeaponPartsComponent OnRep :: Cast Failed PubgCharacter");
	}
}

AWeaponPartsBase* UWeaponPartsComponent::GetPartsInSlot(const FGameplayTag& InSlotTag, bool bGetDefault)
{
	if (IsValid(CurrentWeaponPartsInfo.GetPartsInSlot(InSlotTag)))
	{
		return CurrentWeaponPartsInfo.GetPartsInSlot(InSlotTag);
	}
	return bGetDefault ? DefaultWeaponPartsInfo.GetPartsInSlot(InSlotTag) : nullptr;
}

FString UWeaponPartsComponent::GetSightInfo(float& OutSightFov)
{
	AWeaponSightBase* SightParts = Cast<AWeaponSightBase>(CurrentWeaponPartsInfo.GetPartsInSlot(BaseGameplayTags::Item_Parts_Sight));
	if (IsValid(SightParts))
	{
		OutSightFov = SightParts->GetFov();
		return SightParts->GetSocketName();
	}

	SightParts = Cast<AWeaponSightBase>(DefaultWeaponPartsInfo.GetPartsInSlot(BaseGameplayTags::Item_Parts_Sight));
	if (IsValid(SightParts))
	{
		OutSightFov = SightParts->GetFov();
		return SightParts->GetSocketName();
	}
	
	OutSightFov = 60;
	return FString("Gun_IronSightCameraSocket");
}

void UWeaponPartsComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	
	if (IsValid(DefaultWeaponPartsInfo.MagazineParts))
	{
		DefaultWeaponPartsInfo.MagazineParts->Destroy();
		DefaultWeaponPartsInfo.MagazineParts = nullptr;
	}
	
	if (IsValid(DefaultWeaponPartsInfo.MuzzleParts))
	{
		DefaultWeaponPartsInfo.MuzzleParts->Destroy();
		DefaultWeaponPartsInfo.MuzzleParts = nullptr;
	}
	
	if (IsValid(DefaultWeaponPartsInfo.GripParts))
	{
		DefaultWeaponPartsInfo.GripParts->Destroy();
		DefaultWeaponPartsInfo.GripParts = nullptr;
	}
	
	if (IsValid(DefaultWeaponPartsInfo.SightParts))
	{
		DefaultWeaponPartsInfo.SightParts->Destroy();
		DefaultWeaponPartsInfo.SightParts = nullptr;
	}
	
	if (IsValid(DefaultWeaponPartsInfo.StockParts))
	{
		DefaultWeaponPartsInfo.StockParts->Destroy();
		DefaultWeaponPartsInfo.StockParts = nullptr;
	}
}

TArray<int32> UWeaponPartsComponent::DiscardAllParts()
{
	TArray<int32> CurrentPartsIDs;
	if (IsValid(CurrentWeaponPartsInfo.MagazineParts))
	{
		CurrentPartsIDs.Add(CurrentWeaponPartsInfo.MagazineParts->GetItemID());
		CurrentWeaponPartsInfo.MagazineParts->Destroy();
		CurrentWeaponPartsInfo.MagazineParts = nullptr;
	}
	
	if (IsValid(CurrentWeaponPartsInfo.MuzzleParts))
	{
		CurrentPartsIDs.Add(CurrentWeaponPartsInfo.MuzzleParts->GetItemID());
		CurrentWeaponPartsInfo.MuzzleParts->Destroy();
		CurrentWeaponPartsInfo.MuzzleParts = nullptr;
	}
	
	if (IsValid(CurrentWeaponPartsInfo.GripParts))
	{
		CurrentPartsIDs.Add(CurrentWeaponPartsInfo.GripParts->GetItemID());
		CurrentWeaponPartsInfo.GripParts->Destroy();
		CurrentWeaponPartsInfo.GripParts = nullptr;
	}
	
	if (IsValid(CurrentWeaponPartsInfo.SightParts))
	{
		CurrentPartsIDs.Add(CurrentWeaponPartsInfo.SightParts->GetItemID());
		CurrentWeaponPartsInfo.SightParts->Destroy();
		CurrentWeaponPartsInfo.SightParts = nullptr;
	}
	
	if (IsValid(CurrentWeaponPartsInfo.StockParts))
	{
		CurrentPartsIDs.Add(CurrentWeaponPartsInfo.StockParts->GetItemID());
		CurrentWeaponPartsInfo.StockParts->Destroy();
		CurrentWeaponPartsInfo.StockParts = nullptr;
	}

	return CurrentPartsIDs;
}

FGameplayTag UWeaponPartsComponent::GetMuzzleInfo()
{
	AWeaponPartsBase* MuzzleParts = CurrentWeaponPartsInfo.GetPartsInSlot(BaseGameplayTags::Item_Parts_Muzzle);
	return IsValid(MuzzleParts) ? MuzzleParts->GetEquipSlotTagName() : FGameplayTag::EmptyTag;
}

void UWeaponPartsComponent::OnFppZoomIn()
{
	AWeaponSightBase* SightParts = Cast<AWeaponSightBase>(CurrentWeaponPartsInfo.GetPartsInSlot(BaseGameplayTags::Item_Parts_Sight));
	if (IsValid(SightParts))
	{
		SightParts->OnFPPZoomIN();
	}
}

void UWeaponPartsComponent::OnFppZoomOut()
{
	AWeaponSightBase* SightParts = Cast<AWeaponSightBase>(CurrentWeaponPartsInfo.GetPartsInSlot(BaseGameplayTags::Item_Parts_Sight));
	if (IsValid(SightParts))
	{
		SightParts->OnFPPZoomOut();
	}
}



