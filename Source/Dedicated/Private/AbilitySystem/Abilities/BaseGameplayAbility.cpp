// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/BaseGameplayAbility.h"

#include "Characters/PubgBaseCharacter.h"


UBaseGameplayAbility::UBaseGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

APubgBaseCharacter* UBaseGameplayAbility::GetBaseCharacter() const
{
	return Cast<APubgBaseCharacter>(GetAvatarActorFromActorInfo());
}

void UBaseGameplayAbility::CheckExecuteLocation(EAbilityExecuteLocation& OutExec)
{
	if (HasAuthority(&CurrentActivationInfo))
	{
		OutExec = EAbilityExecuteLocation::CLIENT;
	}
	else
	{
		OutExec = EAbilityExecuteLocation::SERVER;
	}
}
