// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "BaseFireGunAbility.generated.h"


UENUM()
enum class EFireMode : uint8
{
	Single,
	Burst,
	Auto
};

class ABaseGun;
/**
 * 
 */
UCLASS()
class DEDICATED_API UBaseFireGunAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
public:
	UBaseFireGunAbility();
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ABaseGun* CurrentGun;

	float MoaRadian;
	FRandomStream RandomStream;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData = nullptr) override;

	UFUNCTION(BlueprintPure)
	ABaseGun* GetCurrentGun() const { return CurrentGun;}

	UFUNCTION(BlueprintCallable)
	void FireGun();

	virtual void SpawnProjectileFunction();
	void ApplyGunRecoil();
	// void PlayWeaponMontage();

	UFUNCTION(BlueprintCallable, meta=(ExpandEnumAsExecs="OutExec"))
	void CheckFireMode(EFireMode& OutExec);

	UFUNCTION(BlueprintCallable, meta=(ExpandBoolAsExecs="OutExec"))
	void CheckAmmo(bool& OutExec);

};
