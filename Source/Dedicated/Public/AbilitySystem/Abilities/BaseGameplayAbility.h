// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "BaseGameplayAbility.generated.h"

UENUM()
enum class EAbilityExecuteLocation : uint8
{
	CLIENT,
	SERVER
};

class APubgBaseCharacter;
/**
 * 
 */
UCLASS()
class DEDICATED_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UBaseGameplayAbility();
	
	UFUNCTION(BlueprintPure)
	UBaseAbilitySystemComponent* GetBaseAbilitySystemComponent() const {return Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());};

	UFUNCTION(BlueprintPure)
	APubgBaseCharacter* GetBaseCharacter() const;

	UFUNCTION(BlueprintCallable, meta=(ExpandEnumAsExecs="OutExec"))
	void CheckExecuteLocation(EAbilityExecuteLocation& OutExec);
};
