// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UBaseAbilitySystemComponent();

	void TryActivateAbilityByTag(FGameplayTag Tag);
	void SetActiveGameplayEffectLevelByTag(const FGameplayTag& EffectTag, int32 NewLevel);
};
