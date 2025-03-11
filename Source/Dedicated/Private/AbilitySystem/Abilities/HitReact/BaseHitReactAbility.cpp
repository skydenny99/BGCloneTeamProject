// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/HitReact/BaseHitReactAbility.h"


UBaseHitReactAbility::UBaseHitReactAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
}
