// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PubgGameMode.generated.h"

class APubgPlayerController;
class APubgCharacter;
class ABasePlane;
/**
 * 
 */
UCLASS()
class DEDICATED_API APubgGameMode : public AGameMode
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Setting")
	int32 MaxPlayerCount;

	UPROPERTY()
	ABasePlane* LocalPlane;
	
public:
	APubgGameMode();

	// Call start match manually
	virtual bool ReadyToStartMatch_Implementation() override;
	virtual bool ReadyToEndMatch_Implementation() override;
	virtual void HandleMatchHasStarted() override;

	
	
private:
	UPROPERTY(EditAnywhere, Category = "Plane")
	TSubclassOf<ABasePlane> PlaneBlueprintClass;

	
protected:
	
	// PlaneSpawnAndSetViewTarget
	UFUNCTION(Server, Reliable)
	virtual void PlaneStart();

	UFUNCTION(Server, Reliable)
	void ResetPlayerState();
	
};
