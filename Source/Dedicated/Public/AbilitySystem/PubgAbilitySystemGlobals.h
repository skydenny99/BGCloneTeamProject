// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "PubgAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API UPubgAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
public:
	UPubgAbilitySystemGlobals();

	UPROPERTY()
	FGameplayTag GunDamageTag;

	static UPubgAbilitySystemGlobals& PubgGet()
	{
		return dynamic_cast<UPubgAbilitySystemGlobals&>(Get());
	}

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
	virtual void InitGlobalTags() override;
};
