// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpecHandle.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "PubgBaseCharacter.generated.h"


class UBaseTimelineComponent;
class UInventoryManageComponent;
class UPickUpManageComponent;
class UEquipManageComponent;
class UBasePickUpComponent;
class UProjectilePoolComponent;
class UPlayerInventoryComponent;
class UBaseInventoryComponent;
class UBaseGearManageComponent;
class UBaseWeaponManageComponent;
class UDataAsset_PubgInputConfig;
struct FInputActionValue;
class UBaseAbilitySystemComponent;
class UPlayerAttributeSet;
class UDataAsset_StartupBase;

DECLARE_DELEGATE_OneParam(FOnAttributeChangedOneParam, float);
DECLARE_DELEGATE_TwoParams(FOnAttributeChangedTwoParams, float, float);


UCLASS()
class DEDICATED_API APubgBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APubgBaseCharacter();

protected:
	UPROPERTY(Replicated)
	FVector_NetQuantize BaseAimRotation;
	// FRotator BaseAimRotation;

	virtual void BeginPlay() override;

	
#pragma  region Inputs
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacaterData")
	UDataAsset_PubgInputConfig* InputConfigDataAsset;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C130PlaneData")
	//UDataAsset_PubgInputConfig* InputConfigPlaneDataAsset;


protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void Input_Move(const FInputActionValue& InputValue);
	
	// void Input_Look(const FInputActionValue& InputValue);
	void Input_ReleaseMove();
	void OnTriggerAbilityAction(FGameplayTag InputTag);
	// Montage Play Detected
	virtual bool IsMontagePlaying();

	// Stop Montage
	virtual void SkipCurrentMontage();
public:
	virtual FRotator GetBaseAimRotation() const override;
#pragma endregion

#pragma region Abilities
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UBaseAbilitySystemComponent* BaseAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Blueprintreadonly, Category = "AbilitySystem")
	UPlayerAttributeSet* BaseAttributeSet;
public:
	TArray<FGameplayAbilitySpecHandle> GrantAbilities(const TArray<TSubclassOf<UBaseGameplayAbility>>& Abilities);
	void ClearAbilities(const TArray<FGameplayAbilitySpecHandle>& AbilitySpecHandles);

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE UBaseAbilitySystemComponent* GetBaseAbilitySystemComponent() const { return BaseAbilitySystemComponent; }
	FORCEINLINE UPlayerAttributeSet* GetBaseAttributeSet() const { return BaseAttributeSet; }
	
#pragma endregion

#pragma region DataAsset_Startup
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StartupData")
	TSoftObjectPtr<UDataAsset_StartupBase> StartupData;
	
#pragma endregion

#pragma region Components
protected:
	UPROPERTY(EditDefaultsOnly)
	UEquipManageComponent* EquipManageComponent;
	UPROPERTY(EditDefaultsOnly)
	UPickUpManageComponent* PickUpManageComponent;
	UPROPERTY(EditDefaultsOnly)
	UInventoryManageComponent* InventoryComponent;
	UPROPERTY(EditDefaultsOnly)
	UProjectilePoolComponent* ProjectilePoolComponent;
	UPROPERTY(EditDefaultsOnly)
	UBaseTimelineComponent* BaseTimelineComponent;
	
public:
	UFUNCTION(BlueprintPure)
	UEquipManageComponent* GetEquipManageComponent() const { return EquipManageComponent; }
	UFUNCTION(BlueprintPure)
	UPickUpManageComponent* GetPickUpManageComponent() const { return PickUpManageComponent; }
	UFUNCTION(BlueprintPure)
	UInventoryManageComponent* GetInventoryComponent() const { return InventoryComponent; }
	UFUNCTION(BlueprintPure)
	UProjectilePoolComponent* GetProjectilePoolComponent() const {return ProjectilePoolComponent;}
	
#pragma endregion

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(NetMulticast, BlueprintCallable, Reliable)
	virtual void PlayerDead();

	void RegisterOnAttributeChanged_OneParam(const FGameplayAttribute& CurrentData, FOnAttributeChangedOneParam Delegate);

	void RegisterOnAttributeChanged_TwoParams(const FGameplayAttribute& CurrentData, const FGameplayAttribute& MaxData, FOnAttributeChangedTwoParams Delegate);
	
};