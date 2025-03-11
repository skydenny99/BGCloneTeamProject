// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Weapon/BaseGun.h"
#include "GEExec_DamageCalc.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API UGEExec_DamageCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UGEExec_DamageCalc();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:
	float CalculateBodyPartDamageMultiplier(const FName& BoneName) const;
	float CalculateWeaponMultiplier(const EGunType& GunType, const FName& BoneName) const; 
	float CalculateArmorMultiplier(const FName& BoneName, float Armor, float Helmet) const;
};
