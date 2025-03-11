// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Lobby/PubgLobbyGameMode.h"

#include "Characters/PubgCharacter.h"
#include "GameFramework/Lobby/PubgLobbyPlayerController.h"
#include "GameFramework/Lobby/PubgLobbySubsystem.h"


void APubgLobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (UPubgLobbySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UPubgLobbySubsystem>())
	{
		Subsystem->CreateSession(2, true);
	}
}

APubgLobbyGameMode::APubgLobbyGameMode()
{
	DefaultPawnClass = APubgCharacter::StaticClass();
	PlayerControllerClass = APubgLobbyPlayerController::StaticClass();
	
}

bool APubgLobbyGameMode::AllowCheats(APlayerController* P)
{
	bool IsAllowed = (GetNetMode() == NM_Client || GetNetMode() == NM_DedicatedServer || GIsEditor);
	return IsAllowed;
}

void APubgLobbyGameMode::TravelToMainMap()
{
	GetWorld()->ServerTravel("TestMap");
}
