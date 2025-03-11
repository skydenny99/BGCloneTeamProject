// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PubgLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API APubgLobbyGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:
	APubgLobbyGameMode();
	
	virtual bool AllowCheats(APlayerController* P) override;
	void TravelToMainMap();
};
