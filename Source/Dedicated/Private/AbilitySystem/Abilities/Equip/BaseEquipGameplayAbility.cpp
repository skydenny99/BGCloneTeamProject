// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Equip/BaseEquipGameplayAbility.h"

#include "BaseDebugHelper.h"
#include "Characters/PubgBaseCharacter.h"
#include "Components/BaseWeaponManageComponent.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "Weapon/WeaponBase.h"


bool UBaseEquipGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                   const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		UEquipManageComponent* EquipManager = GetBaseCharacter()->GetEquipManageComponent();
		bool bIsDifferentSlot = EquipSlot.MatchesTagExact(EquipManager->GetCurrentWeaponSlotTag()) == false;
		bool bHasWeaponOnTargetSlot = IsValid(EquipManager->GetWeaponInSlot(EquipSlot));
		return bIsDifferentSlot && bHasWeaponOnTargetSlot;
	}
	return false;
}

void UBaseEquipGameplayAbility::IsHoldingWeapon(bool& IsHolding)
{
	IsHolding = GetBaseCharacter()->GetEquipManageComponent()->GetCurrentWeaponSlotTag().MatchesTagExact(BaseGameplayTags::Item_Slot_Punch);
}

void UBaseEquipGameplayAbility::AttachWeaponOnHand()
{
	if (EquipSlot.IsValid() == false) return;
	if (UEquipManageComponent* EquipManager = GetBaseCharacter()->GetEquipManageComponent())
	{
		EquipManager->PullOutWeapon(EquipSlot);
		
	}
}

void UBaseEquipGameplayAbility::DetachWeapon()
{
	if (UEquipManageComponent* EquipManager = GetBaseCharacter()->GetEquipManageComponent())
	{
		EquipManager->PutAwayWeapon();
	}
}
