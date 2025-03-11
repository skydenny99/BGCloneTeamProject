// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "Engine/DataAsset.h"
#include "DataAsset_InputConfig.generated.h"

USTRUCT(BlueprintType)
struct FInputConfig
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction;
};

UCLASS()
class DEDICATED_API UDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FInputConfig> NativeInputConfigs;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FInputConfig> AbilityInputConfigs;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* AttackInputAction;

	UInputAction* GetInputActionByTag(const FGameplayTag& InInputTag);
};
