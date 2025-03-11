// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Weapon/BaseFireModeSelectAbility.h"
#include "Characters/PubgBaseCharacter.h"
#include "Components/BaseWeaponManageComponent.h"
#include "Weapon/BaseGun.h"
#include "Weapon/WeaponBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Components/Renewal/EquipManageComponent.h"

bool UBaseFireModeSelectAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                    const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		const ABaseGun* BaseGun = Cast<ABaseGun>(GetBaseCharacter()->GetEquipManageComponent()->GetCurrentWeapon());
		return IsValid(BaseGun) && BaseGun->GetGunSpec().FireModes.Num() > 1;
	}
	return false;
}

void UBaseFireModeSelectAbility::PreActivate(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
	CurrentGun = Cast<ABaseGun>(GetBaseCharacter()->GetEquipManageComponent()->GetCurrentWeapon());
}


void UBaseFireModeSelectAbility::Pubg_PlayMontageAndWait(UAnimMontage* FireModeSelectMontageToPlay)
{
	if (!FireModeSelectMontageToPlay || !CurrentActorInfo->AvatarActor.IsValid())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	//
	// UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	// if (!ASC)
	// {
	// 	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	// 	return;
	// }

	// PlayMontageAndWaitForEvent 생성
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	this,                 // Ability
	NAME_None,            // TaskInstanceName
	FireModeSelectMontageToPlay,        // MontageToPlay
	1.0f,                 // PlayRate
	NAME_None,            // SectionName
	false,                // bStopWhenAbilityEnds
	1.0f                  // AnimRootMotionTranslationScale
	);
	
	if (Task)
	{
		Task->OnCompleted.AddDynamic(this, &UBaseFireModeSelectAbility::OnMontageCompleted);
		Task->OnCancelled.AddDynamic(this, &UBaseFireModeSelectAbility::OnMontageCancelled);
		Task->OnInterrupted.AddDynamic(this, &UBaseFireModeSelectAbility::OnMontageInterrupted);
		Task->OnBlendOut.AddDynamic(this, &UBaseFireModeSelectAbility::OnMontageBlendOut);
		Task->ReadyForActivation();
	}
	
}

void UBaseFireModeSelectAbility::OnMontageCompleted()
{
	if (HasAuthority(&CurrentActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UBaseFireModeSelectAbility::OnMontageCancelled()
{
	if (HasAuthority(&CurrentActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UBaseFireModeSelectAbility::OnMontageInterrupted()
{
	if (HasAuthority(&CurrentActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UBaseFireModeSelectAbility::OnMontageBlendOut()
{
	if (HasAuthority(&CurrentActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}
