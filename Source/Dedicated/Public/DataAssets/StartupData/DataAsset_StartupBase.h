// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "DataAsset_StartupBase.generated.h"

UCLASS()
class DEDICATED_API UDataAsset_StartupBase : public UDataAsset
{
	GENERATED_BODY()
	
// public:
// 	virtual void GiveToAbilitySystemComponent(UBaseAbilitySystemComponent* ASC, int32 Level =1 );

public:
	UPROPERTY(EditDefaultsOnly, Category = "StartupData")
	TArray<TSubclassOf<UBaseGameplayAbility>> ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartupData")
	TArray<TSubclassOf<UBaseGameplayAbility>> ReactiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartupData")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

	//void GrantAbilities(const TArray<TSubclassOf<UBaseGameplayAbility>> GAs, UBaseAbilitySystemComponent* InASC, int32 Level);
	
	
};
