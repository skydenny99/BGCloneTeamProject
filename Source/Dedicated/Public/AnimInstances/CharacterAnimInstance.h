// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/BaseAnimInstance.h"
#include "CharacterAnimInstance.generated.h"

class UCharacterMovementComponent;
class APubgCharacter;
/**
 * 
 */
UCLASS()
class DEDICATED_API UCharacterAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	APubgCharacter* OwningCharacter;
	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Locomotion")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Locomotion")
	float GroundSpeedWithBack;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Locomotion")
	bool bHasAcceleration;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Locomotion")
	float Direction;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Locomotion")
	float OwningCharacterRunSpeed;
	
	//Inverse Transform Direction And Calculate Direction
	float GetDirection() const;

	float GetBackSpeed() const;

public:
	// SetPlayerMaxWalkSpeed
	void SetOwningSpeed(const float NewSpeed);
};
