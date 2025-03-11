// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/CharacterAnimInstance.h"

#include "Characters/PubgBaseCharacter.h"
#include "Characters/PubgCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"


void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningCharacter = Cast<APubgCharacter>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
		OwningCharacterRunSpeed = OwningCharacter->GetBodyModeSpeed();
	}
}

void UCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!OwningCharacter || !OwningMovementComponent)
		return;

	GroundSpeed = OwningCharacter->GetVelocity().Size2D() / OwningCharacterRunSpeed;
	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;

	GroundSpeedWithBack = GetBackSpeed();

	Direction = GetDirection();
}

float UCharacterAnimInstance::GetDirection() const
{
	if (IsValid(OwningCharacter) == false) return 0.f;

	FVector InverseDirection = UKismetMathLibrary::InverseTransformDirection(OwningCharacter->GetActorTransform(), OwningCharacter->GetVelocity());

	return UKismetAnimationLibrary::CalculateDirection(InverseDirection,FRotator(0.f));
}

float UCharacterAnimInstance::GetBackSpeed() const
{
	float TempSpeed = GroundSpeed;

	FVector TempForward = OwningCharacter->GetActorForwardVector();
	FVector TempDirection = OwningCharacter->GetVelocity().GetSafeNormal();
	float TempDotproduct = FVector::DotProduct(TempForward, TempDirection);
	return (TempDotproduct >= 0.f) ? TempSpeed : -TempSpeed;
}

void UCharacterAnimInstance::SetOwningSpeed(const float NewSpeed)
{
	OwningCharacterRunSpeed = NewSpeed;
}
