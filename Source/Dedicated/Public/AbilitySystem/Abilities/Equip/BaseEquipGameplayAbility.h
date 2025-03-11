// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "BaseEquipGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API UBaseEquipGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Item.Slot"))
	FGameplayTag EquipSlot;
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs="IsHolding"))
	void IsHoldingWeapon(bool& IsHolding);

	UFUNCTION(BlueprintCallable)
	void AttachWeaponOnHand();

	UFUNCTION(BlueprintCallable)
	void DetachWeapon();
};
