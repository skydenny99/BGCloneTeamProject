// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FieldItemManager.generated.h"

UCLASS()
class DEDICATED_API AFieldItemManager : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:	
	// Sets default values for this actor's properties
	AFieldItemManager();

	void InitWorldItems();

	void SpawnItem(int32 ItemID, int32 Amount, const FVector& SpawnLocation);
	
};
