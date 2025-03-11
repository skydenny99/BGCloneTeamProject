// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/PubgAnimInstance.h"

#include "Camera/CameraComponent.h"
#include "Characters/PubgBaseCharacter.h"
#include "Characters/PubgCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"


void UPubgAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	
	if (!OwningMovementComponent)
		return;

	
	Velocity = OwningCharacter->GetVelocity();
	CheckTurnDirection();
	if (bIsFalling != OwningMovementComponent->IsFalling())
		bIsFalling = OwningMovementComponent->IsFalling();
	if (bMoveJump)
	{
		if (!bIsFalling)
			bMoveJump = false;
	}
	
	
	CheckFallingState(DeltaSeconds);
	SetCharacterControlYaw();

	/*if (OwningCharacter->GetZoomIn())
		OwningCharacter->Input_Crouch();*/


	/*PlayerBodyMode = OwningCharacter->GetPlayerBodyMode();
	PlayerEquipMode = OwningCharacter->GetPlayerEquipMode();*/

	/*FString Text = FString::Printf(TEXT("Leaning %F"),mLeaning);
	Debug::PrintWithClientID(GetWorld(), Text,3.f,FColor::Green, -1, 0.f);*/
	
}

void UPubgAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(UPubgAnimInstance, bMoveJump);
	// DOREPLIFETIME(UPubgAnimInstance, bHasTurn);
	// DOREPLIFETIME(UPubgAnimInstance, bSprintProne);
}

void UPubgAnimInstance::CheckTurnDirection()
{
	if(IsValid(OwningCharacter) == false) return;
	if(!OwningCharacter->GetFPP_MODE_Client())
	{
		FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(OwningCharacter->GetBaseAimRotation(), OwningCharacter->GetActorRotation());
	
		if (DeltaRotation.Pitch >= 180.f || DeltaRotation.Yaw >= 180.f)
		{
			Pitch = DeltaRotation.Pitch - 360.f;
			Yaw = DeltaRotation.Yaw - 360.f;
		}
		else
		{
			Pitch = DeltaRotation.Pitch;
			Yaw = DeltaRotation.Yaw;
		}
	}
	else
	{
		if (IsValid(OwningCharacter->GetController()))
		{
			FRotator ControlRotation = OwningCharacter->GetController()->GetControlRotation();
			FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(ControlRotation, OwningCharacter->GetActorRotation());
			Pitch = FMath::UnwindDegrees(DeltaRotation.Pitch);
			Yaw = FMath::UnwindDegrees(DeltaRotation.Yaw);
		}
		
		// Debug::Print(TEXT("Pitch:"), Pitch);
		// Debug::Print(TEXT("Yaw:"), Yaw);
	}
}

void UPubgAnimInstance::CheckFallingState(const float _DeltaSeconds)
{
	if (!bIsFalling && FallingDuration !=0.f)
	{
		FallingDuration = 0.f;
	}

	if (bIsFalling)
		FallingDuration += _DeltaSeconds;

	if (FallingDuration <= 1.f && bIsFalling)
		FallingType = EFallingType::Normal;
	else if (FallingDuration <= 1.5f && FallingDuration >= 1.f && bIsFalling)
		FallingType = EFallingType::Step_One;
	else if (bIsFalling)
		FallingType = EFallingType::Step_Two;

}


void UPubgAnimInstance::SetMoveJump(bool _Bool)
{
	bMoveJump = _Bool;
}

void UPubgAnimInstance::SetCharacterControlYaw()
{
	
	if ((GroundSpeed != 0.f || bIsFalling) && bParachuteExit)
	{
		if (!OwningCharacter->bUseControllerRotationYaw || OwningCharacter->GetZoomIn())
			OwningCharacter->bUseControllerRotationYaw = true;

		if (bHasTurn)
			bHasTurn = false;

		if (GroundSpeed == OwningCharacterRunSpeed)
			OwningCharacter->bUseControllerRotationYaw = true;
	}
	else
	{
		if (OwningCharacter->bUseControllerRotationYaw || !OwningCharacter->GetZoomIn())
			OwningCharacter->bUseControllerRotationYaw = false;
	}
}

