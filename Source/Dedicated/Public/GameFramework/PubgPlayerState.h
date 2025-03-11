// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseType/BaseEnumType.h"
#include "GameFramework/PlayerState.h"
#include "PubgPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API APubgPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Replicated)
	FString PubgPlayerName;
	
	UPROPERTY(ReplicatedUsing=OnRep_UpdateKillCount)
	int32 KillCount = 0;

	UPROPERTY(Replicated)
	FString EliminatedPlayerName;

	UPROPERTY(Replicated)
	int32 PlayerGunID;

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	void OnKill() {KillCount++;}
	int32 GetKillCount() const {return KillCount;}
	FORCEINLINE void SetEliminatedPlayerName(const FString& _TargetName) {EliminatedPlayerName = _TargetName;}
	FORCEINLINE void SetSourcePlayerName(const FString& _SourceName) {PubgPlayerName = _SourceName;}
	FORCEINLINE FString& GetSourcePlayerName() {return PubgPlayerName;}
	FORCEINLINE void SetPlayerGunID(const int32 _GunID) {PlayerGunID = _GunID;}

	UFUNCTION()
	void OnRep_UpdateKillCount();
};
