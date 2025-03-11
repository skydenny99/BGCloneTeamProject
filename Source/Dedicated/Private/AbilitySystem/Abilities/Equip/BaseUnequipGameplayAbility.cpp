// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Equip/BaseUnequipGameplayAbility.h"

#include "Characters/PubgBaseCharacter.h"
#include "Components/BaseWeaponManageComponent.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "Weapon/WeaponBase.h"


bool UBaseUnequipGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                     const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		if (UEquipManageComponent* EquipManager = GetBaseCharacter()->GetEquipManageComponent())
		{
			bool bIsHoldingWeapon = EquipManager->GetCurrentWeaponSlotTag().MatchesTagExact(BaseGameplayTags::Item_Slot_Punch) == false;
			bool bHasLastWeaponInSlot = IsValid(EquipManager->GetWeaponInSlot(EquipManager->GetLastWeaponSlotTag()));
			return bIsHoldingWeapon || bHasLastWeaponInSlot;
		}
	}
	return false;
}

void UBaseUnequipGameplayAbility::IsHoldingWeapon(bool& bIsHoldingWeapon)
{
	UEquipManageComponent* EquipManager = GetBaseCharacter()->GetEquipManageComponent();
	bIsHoldingWeapon = EquipManager->GetCurrentWeaponSlotTag().MatchesTagExact(BaseGameplayTags::Item_Slot_Punch) == false;
	Debug::Print((bIsHoldingWeapon ? "Put away weapon" : "Pull out weapon"));
}

void UBaseUnequipGameplayAbility::PullOutPunch()
{
	if (UEquipManageComponent* EquipManager = GetBaseCharacter()->GetEquipManageComponent())
	{
		EquipManager->PutAwayWeapon();
		EquipManager->PullOutWeapon(BaseGameplayTags::Item_Slot_Punch);
	}
}

void UBaseUnequipGameplayAbility::PullOutLastWeapon()
{
	if (UEquipManageComponent* EquipManager = GetBaseCharacter()->GetEquipManageComponent())
	{
		EquipManager->PutAwayWeapon();
		EquipManager->PullOutWeapon(EquipManager->GetLastWeaponSlotTag());
	}
}