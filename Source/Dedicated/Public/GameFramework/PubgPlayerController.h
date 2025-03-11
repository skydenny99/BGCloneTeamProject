// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "PubgPlayerController.generated.h"

class UCrosshairWidget;
class UPlayerHUD;
struct FGunDamageSpecInfo;
class UGameplayEffect;
class APubgBaseCharacter;

/**
 * 
 */
UCLASS()
class DEDICATED_API APubgPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	
public:
	APubgPlayerController();
	virtual void AcknowledgePossession(APawn* P) override;

	UFUNCTION(Server, Reliable)
	void ApplyDamageToTarget(TSubclassOf<UGameplayEffect> DamageEffectClass, const FHitResult& Hit, const FGunDamageSpecInfo& SpecInfo, APubgBaseCharacter* Target);
	
	UFUNCTION(Server, Reliable)
	void OnPlayerDead();

	UFUNCTION(Server, Reliable)
	void AddGameplayTagToControlledPawn(FGameplayTag Tag);

	UFUNCTION(Server, Reliable)
	void RemoveGameplayTagToControlledPawn(FGameplayTag Tag);
};
