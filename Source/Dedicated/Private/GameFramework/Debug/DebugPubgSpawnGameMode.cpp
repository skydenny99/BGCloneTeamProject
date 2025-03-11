// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Debug/DebugPubgSpawnGameMode.h"

#include "BaseDebugHelper.h"
#include "GameFramework/Debug/DebugPubgPlayerController.h"

ADebugPubgSpawnGameMode::ADebugPubgSpawnGameMode()
{
	PlayerControllerClass = ADebugPubgPlayerController::StaticClass();
	CountdownTime = 5; // 카운트다운 5초 설정
	
}

bool ADebugPubgSpawnGameMode::AllowCheats(APlayerController* P)
{
	bool IsAllowed = (GetNetMode() == NM_Client || GetNetMode() == NM_DedicatedServer || GIsEditor);
	if (IsAllowed)
		Debug::Print("Allowed Cheats");
	return IsAllowed;
}

void ADebugPubgSpawnGameMode::HandleMatchHasStarted()
{
	//플레이어 스폰 
	Super::HandleMatchHasStarted();


	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle, 
		this, 
		&ADebugPubgSpawnGameMode::CountdownTick, 
		1.0f, 
		true // 반복 실행
	);
	
	//LocalPlane->InPlane( LocalPubgController,LocalPubgCharacter);
}

void ADebugPubgSpawnGameMode::CountdownTick()
{
	if (CountdownTime > 0)
	{
		Debug::Print("Match starts in %d seconds...", CountdownTime);
		CountdownTime--;
	}
	else
	{
		// 타이머 정지
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);

		// 0초일 때 매치 시작
		DelayedHandleMatchStart();
	}
}

void ADebugPubgSpawnGameMode::DelayedHandleMatchStart()
{
	Debug::Print("Match has started!");
	// 비행기 시작
	ResetPlayerState();
	PlaneStart();
}


