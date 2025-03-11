// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/PubgGameplayEffectContext.h"

bool FPubgGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	return Super::NetSerialize(Ar, Map, bOutSuccess) && GunDamageSpecInfo.NetSerialize(Ar, Map, bOutSuccess);
}
