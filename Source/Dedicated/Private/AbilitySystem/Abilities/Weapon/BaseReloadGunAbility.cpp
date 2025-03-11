// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Weapon/BaseReloadGunAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/PubgBaseCharacter.h"
#include "Components/BaseWeaponManageComponent.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "Components/Renewal/InventoryManageComponent.h"
#include "HUD/HUDBase.h"
#include "HUD/PlayerHUD/PlayerHUD.h"
#include "Weapon/BaseGun.h"
#include "Weapon/WeaponBase.h"


class UAbilityTask_PlayMontageAndWait;

bool UBaseReloadGunAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                               const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	// 재장전 검사 로직 현재탄환이 최대일때는 재장전 막기
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		const ABaseGun* BaseGun = Cast<ABaseGun>(GetBaseCharacter()->GetEquipManageComponent()->GetCurrentWeapon());
		if (IsValid(BaseGun))
		{
			const bool HasBullet = GetBaseCharacter()->GetInventoryComponent()->GetItemAmount(static_cast<uint8>(BaseGun->GetGunSpec().UsedBullet)) > 0;
			return BaseGun->CanReload() && HasBullet;
		}
		Debug::PrintError("Current Weapon is Not gun or no more valid");
		return false;
	}
	return false;
}

void UBaseReloadGunAbility::PreActivate(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
	CurrentGun = Cast<ABaseGun>(GetBaseCharacter()->GetEquipManageComponent()->GetCurrentWeapon());
}

void UBaseReloadGunAbility::Pubg_PlayMontageAndWait(UAnimMontage* FireModeSelectMontageToPlay)
{

	if (!FireModeSelectMontageToPlay || !CurrentActorInfo->AvatarActor.IsValid())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	// PlayMontageAndWaitForEvent 생성
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	this,                 // Ability
	NAME_None,            // TaskInstanceName
	FireModeSelectMontageToPlay,        // MontageToPlay
	CurrentGun->GetReloadSpeedRate(),                 // PlayRate
	NAME_None,            // SectionName
	false,                // bStopWhenAbilityEnds
	1.0f                  // AnimRootMotionTranslationScale
	);
	
	if (Task)
	{
		Task->OnCompleted.AddDynamic(this, &UBaseReloadGunAbility::OnMontageCompleted);
		Task->OnCancelled.AddDynamic(this, &UBaseReloadGunAbility::OnMontageCancelled);
		Task->OnInterrupted.AddDynamic(this, &UBaseReloadGunAbility::OnMontageInterrupted);
		Task->ReadyForActivation();
	}
}

void UBaseReloadGunAbility::OnMontageCompleted()
{
	if (HasAuthority(&CurrentActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UBaseReloadGunAbility::OnMontageCancelled()
{
	if (HasAuthority(&CurrentActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UBaseReloadGunAbility::OnMontageInterrupted()
{
	if (HasAuthority(&CurrentActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UBaseReloadGunAbility::PlayWeaponMontage()
{
	if (HasAuthority(&CurrentActivationInfo))
	{
		if (IsValid(CurrentGun))
		{
			CurrentGun->NetMulti_PlayReloadMontage();
		}
	}
}
