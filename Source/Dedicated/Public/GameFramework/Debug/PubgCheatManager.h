// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "PubgCheatManager.generated.h"

class ADebugPubgPlayerController;
/**
 * 
 */
UCLASS()
class DEDICATED_API UPubgCheatManager : public UCheatManager
{
	GENERATED_BODY()

	UPROPERTY()
	ADebugPubgPlayerController* PlayerController;
	
	UFUNCTION(Exec)
	void ForceGarbageCollection();

	UFUNCTION(Exec)
	void AlivePlayerCount();

	UFUNCTION(Exec)
	void KillCount();
	// Lobby cheats
	UFUNCTION(Exec)
	void StartMatch();

	// Lobby cheats
	UFUNCTION(Exec)
	void Start();



	// 인벤 테스트
	
	UFUNCTION(Exec)
	void Cheat_AcquireItem(int32 InItemID, int32 InCount);

	UFUNCTION(Exec)
	void Cheat_UseItem(int32 InItemID);

	UFUNCTION(Exec)
	void Cheat_DiscardItem(int32 InItemID, int32 InCount);


	// 픽업 테스트

	UFUNCTION(Exec)
	void Cheat_PrintPickUp();

	UFUNCTION(Exec)
	void Cheat_TryPickUp(int32 InIndex);

	UFUNCTION(Exec)
	void Cheat_Unequip(int32 InIndex);

	UFUNCTION(Exec)
	void Cheat_DiscardPickUp(int32 InItemID, int32 InCount);

	UFUNCTION(Exec)
	void Cheat_AddSight(int32 InItemID);
	
	UFUNCTION(Exec)
	void Cheat_RemoveSight();
	
	UFUNCTION(Exec)
	void Cheat_RemoveMuzzle();

	UFUNCTION(Exec)
	void Cheat_RemoveMagazine();
	

	UFUNCTION(Exec)
	void Cheat_CheckPartsAvailable(int32 InItemID);

	// 자기장
	UFUNCTION(Exec)
	void Cheat_SetBluezoneLevel(int32 InLevel);

	UFUNCTION(Exec)
	void Cheat_SetBluezoneRadius(float InRadius, float InPeriod);

	UFUNCTION(Exec)
	void Cheat_SetBluezoneRound(int32 InRound);

	// 디버그 체크
	UFUNCTION(Exec)
	void Cheat_WeaponManage();
};
