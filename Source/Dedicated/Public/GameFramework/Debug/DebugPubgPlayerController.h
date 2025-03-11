// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PubgPlayerController.h"
#include "DebugPubgPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API ADebugPubgPlayerController : public APubgPlayerController
{
	GENERATED_BODY()
protected:
	virtual void AddCheats(bool bForce = false) override;
public:
	ADebugPubgPlayerController();

	UFUNCTION(Server, Reliable)
	void StartGame();

	UFUNCTION(Server, Reliable)
	void SetBluezoneLevel(int32 NewLevel);

	UFUNCTION(Server, Reliable)
	void SetBluezoneRadius(float NewRadius, float InPeriod);

	UFUNCTION(Server, Reliable)
	void SetBluezoneRound(int32 InNewRound);

	UFUNCTION(Server, Reliable)
	void Server_CheckCurrentWeapon();

	UFUNCTION(Server, Reliable)
	void Server_AddSight(int32 InItemID);

	UFUNCTION(Server, Reliable)
	void Server_RemoveSight();

	UFUNCTION(Server, Reliable)
	void Server_RemoveMuzzle();
	
	UFUNCTION(Server, Reliable)
	void Server_RemoveMagazine();

	
	UFUNCTION(Server, Reliable)
	void Server_CheckPartsAvailable(int32 InItemID);
};
