// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PubgGameMode.h"
#include "DebugPubgSpawnGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API ADebugPubgSpawnGameMode : public APubgGameMode
{
	GENERATED_BODY()
	
public:
	ADebugPubgSpawnGameMode();

	virtual bool AllowCheats(APlayerController* P) override;

	
	
	virtual void HandleMatchHasStarted() override;
	
private:
	void CountdownTick();  // 카운트다운 함수
	void DelayedHandleMatchStart(); // 5초 뒤 실행될 함수

	FTimerHandle CountdownTimerHandle;  // 카운트다운 타이머
	FTimerHandle MatchStartTimerHandle; // 매치 시작 타이머

	int CountdownTime = 5; // 5초 카운트다운
};
