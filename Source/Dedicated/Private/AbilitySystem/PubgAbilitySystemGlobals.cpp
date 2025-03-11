// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/PubgAbilitySystemGlobals.h"

#include "BaseGameplayTags.h"
#include "AbilitySystem/PubgGameplayEffectContext.h"


UPubgAbilitySystemGlobals::UPubgAbilitySystemGlobals()
{
}

FGameplayEffectContext* UPubgAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FPubgGameplayEffectContext();
}

void UPubgAbilitySystemGlobals::InitGlobalTags()
{
	Super::InitGlobalTags();

	GunDamageTag = BaseGameplayTags::Shared_Data_GunDamage;
}
