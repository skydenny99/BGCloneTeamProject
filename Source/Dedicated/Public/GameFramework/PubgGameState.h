// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PubgGameState.generated.h"

class ABaseBluezone;
/**
 * 
 */
UCLASS()
class DEDICATED_API APubgGameState : public AGameState
{
	GENERATED_BODY()

public:
	APubgGameState();

protected:
	UPROPERTY()
	ABaseBluezone* BlueZone;

public:
	// 블루존 스폰 파라미터
	UPROPERTY()
	FVector SpawnLocation;
	UPROPERTY()
	FRotator SpawnRotation;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseBluezone> BlueZoneClass;

	UPROPERTY(ReplicatedUsing=OnRep_PlayerCount)
	int32 AlivePlayerCount;

	UPROPERTY(Replicated)
	int32 TotalPlayerCount;


	UPROPERTY(Replicated)
	int32 BlueZoneRound = 1;

	float CurrentBlueZoneRadius;
	float TargetBlueZoneRadius;
	FVector CurrentBlueZoneLocation;
	FVector TargetBlueZoneLocation;
	double BlueZoneStartTime;
	float BlueZonePeriod;
	FTimerHandle BlueZoneTimer;

	void UpdateCurrentBlueZone();
	virtual void HandleMatchHasStarted() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	bool GetGameElapsedSecs(int32& OutGameElapsedSecs) const;
	void InitAlivePlayerCount() { AlivePlayerCount = TotalPlayerCount = PlayerArray.Num(); }
	int32 GetAlivePlayerCount() const { return AlivePlayerCount; }
	void OnPlayerDead() { AlivePlayerCount--; }

	void UpdateBlueZoneLevel(int32 BlueZoneLevel);
	void UpdateBlueZone(int32 InBlueZoneRound);
	void StartNextBlueZone();

	UFUNCTION()
	void OnRep_PlayerCount();

	FORCEINLINE int32 GetTotalPlayerCount() const { return TotalPlayerCount; }
};

inline void APubgGameState::StartNextBlueZone()
{
	UpdateBlueZone(BlueZoneRound++);
}
