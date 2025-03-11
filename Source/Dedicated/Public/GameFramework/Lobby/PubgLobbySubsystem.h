// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "PubgLobbySubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreatePubgSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartPubgSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroyPubgSessionComplete, bool, Successful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFindPubgSessionComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnJoinPubgSessionComplete, EOnJoinSessionCompleteResult::Type Result);

/**
 * 
 */
UCLASS()
class DEDICATED_API UPubgLobbySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UPubgLobbySubsystem();

	void CreateSession(int32 NumPublicConnections, bool IsLANMatch);
	void StartSession();
	void DestroySession();
	void FindSession(int32 MaxSearchCount, bool IsLANQuery);
	void JoinSession(const FOnlineSessionSearchResult& SessionResult);
	
	FOnCreatePubgSessionComplete OnCreateSessionCompleteDelegate;
	FOnStartPubgSessionComplete OnStartSessionCompleteDelegate;
	FOnDestroyPubgSessionComplete OnDestroySessionCompleteDelegate;
	FOnFindPubgSessionComplete OnFindSessionCompleteDelegate;
	FOnJoinPubgSessionComplete OnJoinSessionCompleteDelegate;

protected:
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnStartSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	bool TryTravelToCurrentSession();

private:
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;

	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
};
