// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Lobby/PubgLobbyPlayerController.h"

#include "BaseDebugHelper.h"
#include "GameFramework/Debug/PubgCheatManager.h"
#include "GameFramework/Lobby/PubgLobbyGameMode.h"
#include "GameFramework/Lobby/PubgLobbySubsystem.h"


// void APubgLobbyPlayerController::StartGame_Implementation()
// {
// 	if (APubgLobbyGameMode* GameMode = Cast<APubgLobbyGameMode>(GetWorld()->GetAuthGameMode()))
// 	{
// 		Debug::Print("StartGame_Implementation");
// 		GameMode->TravelToMainMap();
// 	}
// }
//
// void APubgLobbyPlayerController::AddCheats(bool bForce)
// {
// 	Super::AddCheats(true);
// }
//
// void APubgLobbyPlayerController::BeginPlay()
// {
// 	Super::BeginPlay();
// 	if (UPubgLobbySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UPubgLobbySubsystem>())
// 	{
// 		Subsystem->OnFindSessionCompleteDelegate.AddUObject(this, &APubgLobbyPlayerController::OnFindSession);
// 	}
// }
//
// APubgLobbyPlayerController::APubgLobbyPlayerController()
// {
// 	CheatClass = UPubgCheatManager::StaticClass();
// }
//
// void APubgLobbyPlayerController::OnFindSession(const TArray<FOnlineSessionSearchResult>& InResults, bool Success)
// {
// 	SessionSearchResults.Empty();
// 	if (Success)
// 	{
// 		SessionSearchResults = InResults;
// 	}
// }
//
// void APubgLobbyPlayerController::JoinSession()
// {
// 	if (SessionSearchResults.IsEmpty()) return;
// 	if (UPubgLobbySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UPubgLobbySubsystem>())
// 	{
// 		Subsystem->JoinSession(SessionSearchResults[0]);
// 	}
// }

