// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PubgGameMode.h"

#include "BaseDebugHelper.h"
#include "Characters/PubgCharacter.h"
#include "GameFramework/PubgGameState.h"
#include "GameFramework/PubgPlayerController.h"
#include "GameFramework/PubgPlayerState.h"
#include "Spawn/BasePlane.h"


APubgGameMode::APubgGameMode()
{
	DefaultPawnClass = APubgCharacter::StaticClass();
	GameStateClass = APubgGameState::StaticClass();
	PlayerControllerClass = APubgPlayerController::StaticClass();
	PlayerStateClass = APubgPlayerState::StaticClass();

	bDelayedStart = true;
}

bool APubgGameMode::ReadyToStartMatch_Implementation()
{
	// If bDelayed Start is set, wait for a manual match start
	if (bDelayedStart)
	{
		return false;
	}

	// By default start when we have > 0 players
	if (GetMatchState() == MatchState::WaitingToStart)
	{
		if (NumPlayers > 0)
		{
			return true;
		}
	}
	return false;
}

bool APubgGameMode::ReadyToEndMatch_Implementation()
{
	return Super::ReadyToEndMatch_Implementation();
}

void APubgGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	if (APubgGameState* PubgGameState = Cast<APubgGameState>(GameState))
	{
		PubgGameState->InitAlivePlayerCount();
	}
	
	Debug::Print("StartMatch");
	
	//비행기 시작
	//PlaneStart();
}
void APubgGameMode::PlaneStart_Implementation()
{
	// 비행기 스폰
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(FVector(0.0f, 0.0f, 4000.0f)); // 스폰 위치
	SpawnTransform.SetRotation(FQuat::Identity);
	SpawnTransform.SetScale3D(FVector(1.0f));

	if (PlaneBlueprintClass)
	{
		ABasePlane* SpawnedPlane = GetWorld()->SpawnActor<ABasePlane>(
			PlaneBlueprintClass,
			SpawnTransform
		);

		if (!SpawnedPlane)
		{
			Debug::Print("Plane could not be spawned.");
			return;
		}
		LocalPlane = SpawnedPlane;
		Debug::Print("PlaneStart");
		
		for (const APlayerState* PlayerState  : GameState->PlayerArray)
		{
			UE_LOG(LogTemp, Error, TEXT("CheckActor"));
			
			// PlayerState를 확인
			if (!PlayerState)
			{
				Debug::Print("Invalid PlayerState.");
				continue;
			}
			const APlayerController* PlayerController = Cast<APlayerController>(PlayerState->GetOwner());
			if (!PlayerController)
			{
				Debug::Print("Invalid PlayerController.");
				continue;
			}
			APubgCharacter* PubgPlayerCharacter = Cast<APubgCharacter>(PlayerController->GetPawn());
			if (!PubgPlayerCharacter)
			{
				Debug::Print("Invalid PlayerCharacter.");
				continue;
			}

			PubgPlayerCharacter -> AttachPlane();
			PubgPlayerCharacter->Client_CrosshairTrigger(false);

			
			FString DebugText = FString::Printf(TEXT("Set ViewTarget for PlayerController To Plane %s"), *PlayerController->GetName());
			Debug::Print(DebugText, FColor::Blue,-1,2.f);
		}
	}
	else
	{
		Debug::Print("PlaneBlueprintClass is null.");
	}
}
void APubgGameMode::ResetPlayerState_Implementation()
{
	for (const APlayerState* PlayerState  : GameState->PlayerArray)
	{
		UE_LOG(LogTemp, Error, TEXT("CheckActor"));
			
		// PlayerState를 확인
		if (!PlayerState)
		{
			Debug::Print("Invalid PlayerState.");
			continue;
		}
		const APlayerController* PlayerController = Cast<APlayerController>(PlayerState->GetOwner());
		if (!PlayerController)
		{
			Debug::Print("Invalid PlayerController.");
			continue;
		}
		APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(PlayerController->GetPawn());
		if (!PlayerCharacter)
		{
			Debug::Print("Invalid PlayerCharacter.");
			continue;
		}
		// 플레이어 무브먼트 리셋
		if (UCharacterMovementComponent* MovementComp = PlayerCharacter->GetCharacterMovement())
		{
			MovementComp->StopMovementImmediately();
			MovementComp->SetMovementMode(MOVE_None); // 이동 불가 상태
		}
	}
	// 애니메이션 및 기타 상태 초기화 필요하면 추가
}
