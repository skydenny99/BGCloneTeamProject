// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/BaseAnimInstance.h"
#include "BaseType/BaseEnumType.h"
#include "InventoryCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API UInventoryCharacterAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetEquipMode(const EPlayerActionMode _EquipMode) {AnimEquipMode = _EquipMode;}
	FORCEINLINE EPlayerActionMode GetEquipMode() const {return AnimEquipMode;}
	
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	EPlayerActionMode AnimEquipMode = EPlayerActionMode::Unarmed;
	
	
};
