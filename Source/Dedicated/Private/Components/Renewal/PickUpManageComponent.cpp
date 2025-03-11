// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Renewal/PickUpManageComponent.h"

#include "BaseDebugHelper.h"
#include "BaseGameplayTags.h"
#include "Characters/PubgCharacter.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "Components/Renewal/InventoryManageComponent.h"
#include "Gear/BaseGear.h"
#include "Item/BaseFieldItem.h"
#include "Item/BaseGearFieldItem.h"
#include "Item/BaseWeaponFieldItem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/WeaponBase.h"


// Sets default values for this component's properties
UPickUpManageComponent::UPickUpManageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPickUpManageComponent::InternalPickUpItem(ABaseFieldItem* InFieldItem)
{
	if (IsValid(InFieldItem) == false) return;
	if (IsValid(GetOwner()) &&  GetOwner()->HasAuthority())
	{
		if (UInventoryManageComponent* Inventory = GetOwner()->FindComponentByClass<UInventoryManageComponent>())
		{
			int32 OutRemainAmount = 0;
			if (Inventory->AcquireItem(InFieldItem->GetItemID(), InFieldItem->GetItemAmount(), OutRemainAmount))
			{
				if (OutRemainAmount == 0)
				{
					InFieldItem->Destroy();
				}
				else
				{
					InFieldItem->SetAmount(OutRemainAmount);
				}
			}
		}
		else
		{
			Debug::PrintError("Character needs Inventory Manage Component");
		}
	}
}

void UPickUpManageComponent::InternalPickUpWeapon(ABaseWeaponFieldItem* InFieldWeapon, const FGameplayTag& InSlotTag)
{
	if (IsValid(InFieldWeapon) == false) return;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		if (InFieldWeapon->GetWeaponClass())
		{
			if (UEquipManageComponent* EquipManager = GetOwner()->FindComponentByClass<UEquipManageComponent>())
			{
				const FGameplayTag EquipSlot = InSlotTag.IsValid() ? InSlotTag : EquipManager->GetAvailableWeaponSlot(InFieldWeapon->GetEquippableTags());
				if (EquipSlot.IsValid())
				{
					if (AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(InFieldWeapon->GetWeaponClass()))
					{
						InFieldWeapon->OverrideWeaopnSpec(Weapon);
						DiscardWeapon(EquipManager->UnEquipWeapon(EquipSlot));
						EquipManager->EquipWeapon(Weapon, EquipSlot);
						Weapon->SetOwner(GetOwner());
						InFieldWeapon->Destroy();
					}
				}
			}
			else
			{
				Debug::PrintError("Character needs Equip Manage Component");
			}
		}
		else
		{
			Debug::PrintError(FString::Printf(TEXT("No weapon class in %s"), *InFieldWeapon->GetActorNameOrLabel()));
		}
	}
}

void UPickUpManageComponent::InternalPickUpGear(ABaseGearFieldItem* InFieldGear)
{
	if (IsValid(InFieldGear) == false) return;
	if (IsValid(GetOwner()) &&  GetOwner()->HasAuthority())
	{
		if (InFieldGear->GetGearClass())
		{
			if (UEquipManageComponent* EquipManager = GetOwner()->FindComponentByClass<UEquipManageComponent>())
			{
				if (ABaseGear* Gear = GetWorld()->SpawnActor<ABaseGear>(InFieldGear->GetGearClass()))
				{
					DiscardGear(EquipManager->UnEquipGear(Gear->GetGearType()));
					EquipManager->EquipGear(Gear);
					InFieldGear->Destroy();
				}
			}
			else
			{
				Debug::PrintError("Character needs Equip Manage Component");
			}
		}
		else
		{
			Debug::PrintError(FString::Printf(TEXT("No Gear class in %s"), *InFieldGear->GetActorNameOrLabel()));
		}
	}
}

void UPickUpManageComponent::InternalPickUpParts(ABaseFieldItem* InFieldParts, const FGameplayTag& InWeaponSlotTag)
{
	if (IsValid(InFieldParts) == false) return;
	if (IsValid(GetOwner()) &&  GetOwner()->HasAuthority())
	{
		if (UEquipManageComponent* EquipManager = GetOwner()->FindComponentByClass<UEquipManageComponent>())
		{
			if (AWeaponBase* Weapon = EquipManager->GetWeaponInSlot(InWeaponSlotTag))
			{
				const int32 PartsItemID = InFieldParts->GetItemID();
				if (Weapon->IsEquippableParts(PartsItemID))
				{
					Server_DiscardItem_Implementation(Weapon->UnequipWeaponParts(InFieldParts->GetItemType()), 1);
					Weapon->EquipWeaponParts(PartsItemID);
					InFieldParts->Destroy();
				}
				else
				{
					Debug::PrintError("Parts is not equippable in slot. Parts ID", PartsItemID);
				}
			}
		}
	}
}

void UPickUpManageComponent::DiscardGear(ABaseGear* InGear)
{
	if (IsValid(InGear) == false) return;
	if (IsValid(GetOwner()) &&  GetOwner()->HasAuthority())
	{
		const float CurrentDur = InGear->GetCurrentDurability();
		const float MaxDur = InGear->GetMaxDurability();
		if (ABaseGearFieldItem* FieldItem = SpawnFieldItemByItemID<ABaseGearFieldItem>(InGear->GetGearItemID()))
		{
			FieldItem->SetAmount(1);
			FieldItem->SetWorldSpawned(true);
			FieldItem->SetDurability(CurrentDur, MaxDur);
			FieldItem->SnapOnGround();
		}
		InGear->Destroy();
	}
}

void UPickUpManageComponent::DiscardWeapon(AWeaponBase* InWeapon)
{
	if (IsValid(InWeapon) == false) return;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		if (ABaseWeaponFieldItem* FieldItem = SpawnFieldItemByItemID<ABaseWeaponFieldItem>(InWeapon->GetItemID()))
		{
			FieldItem->SetAmount(1);
			FieldItem->SetWorldSpawned(true);
			FieldItem->InitWeaponSpec(InWeapon);
			FieldItem->SnapOnGround();
		}
		const TArray<int32> PartsIDs = InWeapon->GetAllWeaponPartsIDs();
		for (auto& PartsID : PartsIDs)
		{
			if (PartsID > 0)
			{
				if (ABaseFieldItem* FieldItem = SpawnFieldItemByItemID<ABaseFieldItem>(PartsID))
				{
					FieldItem->SetAmount(1);
					FieldItem->SetWorldSpawned(true);
					FieldItem->SnapOnGround();
				}
			}
		}
		InWeapon->Destroy();
	}
}

void UPickUpManageComponent::DiscardWeaponParts(AWeaponBase* InWeapon, const FGameplayTag& InPartsSlotTag)
{
	if (IsValid(InWeapon) == false) return;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		const int32 PartsItemID = InWeapon->UnequipWeaponParts(InPartsSlotTag);
		if (PartsItemID > 0)
		{
			if (ABaseFieldItem* FieldItem = SpawnFieldItemByItemID<ABaseFieldItem>(PartsItemID))
			{
				FieldItem->SetAmount(1);
				FieldItem->SetWorldSpawned(true);
				FieldItem->SnapOnGround();
			}
		}
	}
}

EPickMode UPickUpManageComponent::GetPickMode(const float LocationZ) const
{
	const float OwnerZ = GetOwner()->GetActorLocation().Z;
	if (UKismetMathLibrary::Abs( OwnerZ - LocationZ) < 20.f)
		return EPickMode::Middle;
	if (OwnerZ > LocationZ)
		return EPickMode::Low;
	return EPickMode::High;
}

void UPickUpManageComponent::AddPickUpCandidate(ABaseFieldItem* InFieldItem)
{
	PickUpCandidates.AddUnique(InFieldItem);
}

void UPickUpManageComponent::RemovePickUpCandidate(ABaseFieldItem* InFieldItem)
{
	PickUpCandidates.Remove(InFieldItem);
}

void UPickUpManageComponent::PickUp(ABaseFieldItem* InFieldItem)
{
	if (IsValid(InFieldItem) && IsValid(GetOwner()) && GetOwner<APawn>()->IsLocallyControlled())
	{
		if (PickUpCandidates.Contains(InFieldItem))
		{
			Server_PickUp(InFieldItem, true);
			PickUpCandidates.Remove(InFieldItem);
		}
	}
}

void UPickUpManageComponent::Server_DiscardItem_Implementation(int32 InItemID, int32 InAmount)
{
	if (IsValid(GetOwner()) == false || InAmount <= 0 || InItemID <= 0) return;
	if (UInventoryManageComponent* Inventory = GetOwner()->FindComponentByClass<UInventoryManageComponent>())
	{
		const int32 DiscardAmount = Inventory->DiscardItem(InItemID, InAmount);
		if (DiscardAmount == 0) return;
		if (ABaseFieldItem* FieldItem = SpawnFieldItemByItemID<ABaseFieldItem>(InItemID))
		{
			FieldItem->SetAmount(DiscardAmount);
			FieldItem->SetWorldSpawned(true);
			FieldItem->SnapOnGround();
		}
	}
	else
	{
		Debug::PrintError("Character needs Inventory Manage Component");
	}
}

void UPickUpManageComponent::Server_DiscardGear_Implementation(const FGameplayTag& InSlotTag)
{
	if (IsValid(GetOwner()) == false || InSlotTag.IsValid() == false) return;
	if (UEquipManageComponent* EquipManager = GetOwner()->FindComponentByClass<UEquipManageComponent>())
	{
		DiscardGear(EquipManager->UnEquipGear(InSlotTag));
	}
	else
	{
		Debug::PrintError("Character needs Equip Manage Component");
	}
}

void UPickUpManageComponent::Server_DiscardWeapon_Implementation(const FGameplayTag& InSlotTag)
{
	if (IsValid(GetOwner()) == false || InSlotTag.IsValid() == false) return;
	if (UEquipManageComponent* EquipManager = GetOwner()->FindComponentByClass<UEquipManageComponent>())
	{
		DiscardWeapon(EquipManager->UnEquipWeapon(InSlotTag));
	}
	else
	{
		Debug::PrintError("Character needs Equip Manage Component");
	}
}

void UPickUpManageComponent::Server_PickUp_Implementation(ABaseFieldItem* InFieldItem, bool bNeedToPlayMontage)
{
	if (IsValid(InFieldItem) == false) return;

	if (bNeedToPlayMontage)
	{
		if (APubgCharacter* PubgCharacter = GetOwner<APubgCharacter>())
		{
			PubgCharacter->PlayPickUpMontage(GetPickMode(InFieldItem->GetActorLocation().Z));
		}
	}
	
	if (ABaseGearFieldItem* GearFieldItem = Cast<ABaseGearFieldItem>(InFieldItem))
	{
		InternalPickUpGear(GearFieldItem);
	}
	else if (ABaseWeaponFieldItem* WeaponFieldItem = Cast<ABaseWeaponFieldItem>(InFieldItem))
	{
		InternalPickUpWeapon(WeaponFieldItem);
	}
	else
	{
		InternalPickUpItem(InFieldItem);
	}
}

void UPickUpManageComponent::Server_PickUpWithSlot_Implementation(ABaseFieldItem* InFieldItem, const FGameplayTag& SlotTag)
{
	if (IsValid(InFieldItem) == false || SlotTag.IsValid() == false) return;
	
	if (APubgCharacter* PubgCharacter = GetOwner<APubgCharacter>())
	{
		PubgCharacter->PlayPickUpMontage(GetPickMode(InFieldItem->GetActorLocation().Z));
	}
	
	if (ABaseGearFieldItem* GearFieldItem = Cast<ABaseGearFieldItem>(InFieldItem))
	{
		if (GearFieldItem->GetEquippableSlot().MatchesTagExact(SlotTag))
		{
			InternalPickUpGear(GearFieldItem);
		}
	}

	if (ABaseWeaponFieldItem* WeaponFieldItem = Cast<ABaseWeaponFieldItem>(InFieldItem))
	{
		if (WeaponFieldItem->GetEquippableTags().HasTagExact(SlotTag))
		{
			InternalPickUpWeapon(WeaponFieldItem, SlotTag);
		}
	}
	
	if (InFieldItem->GetItemType().MatchesTag(BaseGameplayTags::Item_Parts))
	{
		if (SlotTag.MatchesTag(BaseGameplayTags::Item_Slot))
		{
			InternalPickUpParts(InFieldItem, SlotTag);
		}
	}
	
}

void UPickUpManageComponent::Server_DiscardWeaponParts_Implementation(const FGameplayTag& InWeaponSlotTag,
	const FGameplayTag& InPartsSlotTag)
{
	
	if (IsValid(GetOwner()) == false || InWeaponSlotTag.IsValid() == false) return;
	if (UEquipManageComponent* EquipManager = GetOwner()->FindComponentByClass<UEquipManageComponent>())
	{
		if (IsValid(EquipManager->GetWeaponInSlot(InWeaponSlotTag)))
		{
			DiscardWeaponParts(EquipManager->GetWeaponInSlot(InWeaponSlotTag), InPartsSlotTag);
		}
	}
	else
	{
		Debug::PrintError("Character needs Equip Manage Component");
	}
}

void UPickUpManageComponent::DiscardAllItems()
{
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		if (UInventoryManageComponent* Inventory = GetOwner()->FindComponentByClass<UInventoryManageComponent>())
		{
			for (auto ItemIter = Inventory->GetItemsRef().CreateIterator(); ItemIter; ++ItemIter)
			{
				if (ABaseFieldItem* FieldItem = SpawnFieldItemByItemID<ABaseFieldItem>(ItemIter->ItemID))
				{
					FieldItem->SetAmount(ItemIter->ItemAmount);
					FieldItem->SnapOnGround();
				}
				ItemIter.RemoveCurrent();
			}
			Inventory->ClearItems();
		}

		if (UEquipManageComponent* EquipManager = GetOwner()->FindComponentByClass<UEquipManageComponent>())
		{
			const FGameplayTag SlotParent = FGameplayTag::RequestGameplayTag("Item.Slot");
			const FGameplayTagContainer ItemSlotContainer = UGameplayTagsManager::Get().RequestGameplayTagChildren(SlotParent);
			for (auto& SlotTag : ItemSlotContainer)
			{
				DiscardGear(EquipManager->GetGearInSlot(SlotTag));
				DiscardWeapon(EquipManager->GetWeaponInSlot(SlotTag));
			}
		}
	}
}
