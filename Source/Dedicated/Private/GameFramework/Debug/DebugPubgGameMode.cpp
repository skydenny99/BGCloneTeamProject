// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Debug/DebugPubgGameMode.h"

#include "BaseDebugHelper.h"
#include "GameFramework/Debug/DebugPubgPlayerController.h"


ADebugPubgGameMode::ADebugPubgGameMode()
{
	PlayerControllerClass = ADebugPubgPlayerController::StaticClass();
}

bool ADebugPubgGameMode::AllowCheats(APlayerController* P)
{
	bool IsAllowed = (GetNetMode() == NM_Client || GetNetMode() == NM_DedicatedServer || GIsEditor);
	if (IsAllowed)
		Debug::Print("Allowed Cheats");
	return IsAllowed;
}

