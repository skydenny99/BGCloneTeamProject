// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponAnimInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API UWeaponAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	float FireMode = 1.f;
	
	
};
