// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "BaseUnequipGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API UBaseUnequipGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs="bIsHoldingWeapon"))
	void IsHoldingWeapon(bool& bIsHoldingWeapon);

	UFUNCTION(BlueprintCallable)
	void PullOutPunch();

	UFUNCTION(BlueprintCallable)
	void PullOutLastWeapon();
};
