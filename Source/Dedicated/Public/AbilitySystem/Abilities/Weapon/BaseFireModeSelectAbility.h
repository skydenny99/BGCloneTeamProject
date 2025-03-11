// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "Weapon/BaseGun.h"
#include "BaseFireModeSelectAbility.generated.h"

class AWeaponBase;
/**
 * 
 */
UCLASS()
class DEDICATED_API UBaseFireModeSelectAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ABaseGun* CurrentGun;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData = nullptr) override;
	
	UFUNCTION(BlueprintPure)
	ABaseGun* GetCurrentGun() const { return CurrentGun;}
	
	
	UFUNCTION(BlueprintCallable)
	void Pubg_PlayMontageAndWait(UAnimMontage* FireModeSelectMontageToPlay);
	
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageBlendOut();
	
};
