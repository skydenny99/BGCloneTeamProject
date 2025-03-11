// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BaseFunctionLibrary.generated.h"

struct FGunSpec;
struct FInventoryDataStruct;
class UBaseAbilitySystemComponent;
class UBaseGameplayAbility;
class UBaseAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class DEDICATED_API UBaseFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UBaseAbilitySystemComponent* NativeGetBaseAbilitySystemComponentFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static void AddPlaygameTagToActor(AActor* Actor, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static void RemovePlayGameTagFromActor(AActor* Actor, FGameplayTag Tag);

	static bool NativeActorHasTag(AActor* Actor, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static bool BP_NativeActorHasTag(AActor* Actor, FGameplayTag Tag);

	static bool NativeActorHasAnyTags(AActor* Actor, FGameplayTagContainer Tags);
	
	static FRandomStream SetSeedWithActivationKey(UBaseGameplayAbility* Ability);

	UFUNCTION(BlueprintPure, Category = "FunctionLibrary")
	static float GetRandomFloat(UBaseGameplayAbility* Ability);
	UFUNCTION(BlueprintPure, Category = "FunctionLibrary")
	static float GetRandomFloatRange(UBaseGameplayAbility* Ability, float Min, float Max);
	
	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static FGameplayAbilityTargetDataHandle MakeTargetDataHandleFromTransform(const FTransform& InTransform);

	UFUNCTION(BlueprintPure, Category = "FunctionLibrary")
	static FVector MakeCharacterAimVector(ACharacter* PlayerCharacter);

	static const FInventoryDataStruct* GetItemData(int32 InItemID);
	static const FGunSpec* GetGunData(int32 InItemID);
};
