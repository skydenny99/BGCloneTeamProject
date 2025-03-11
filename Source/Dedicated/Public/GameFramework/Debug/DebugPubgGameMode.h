// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PubgGameMode.h"
#include "DebugPubgGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API ADebugPubgGameMode : public APubgGameMode
{
	GENERATED_BODY()
	
public:
	ADebugPubgGameMode();

	virtual bool AllowCheats(APlayerController* P) override;
	
};
