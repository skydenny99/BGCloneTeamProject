// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Weapon/BaseFireGunAbility.h"
#include "BaseFireShotgunAbility.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API UBaseFireShotgunAbility : public UBaseFireGunAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 PelletCount = 6;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PelletSpreadDegree = 10.0f;
	float PelletSpreadRadian;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void SpawnProjectileFunction() override;
	
};
