// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"


class AWeaponPartsBase;
class UWeaponPartsComponent;
class ABaseWeaponFieldItem;
class APubgBaseCharacter;
class UAbilitySystemComponent;
class AWeaponBase;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateWeaponInfo, AWeaponBase*)


UCLASS()
class DEDICATED_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* WeaponMeshComp;

	UPROPERTY(EditDefaultsOnly)
	UWeaponPartsComponent* WeaponPartsComponent;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UBaseGameplayAbility>> WeaponGrantAbilities;
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories = "Item.Slot"))
	FGameplayTagContainer WeaponEquipSlot;

	UPROPERTY(EditDefaultsOnly, Category="ItemInfo")
	int32 ItemID;

	

public:
	FOnUpdateWeaponInfo OnUpdateWeaponInfoDelegate;
	
	virtual bool CanAttack() const {return true;}
	virtual void WeaponReload() {}
	virtual void GrantAbilities(APubgBaseCharacter* AbilityOwner);
	virtual void ClearAbilities(APubgBaseCharacter* AbilityOwner);
	virtual void OnPullOut();
	virtual void OnPutAway();

	bool IsEquippableParts(int32 InItemID) const;
	void EquipWeaponParts(int32 InItemID);
	int32 UnequipWeaponParts(const FGameplayTag& InSlotTag);
	TArray<int32> GetAllWeaponPartsIDs();

	UFUNCTION(BlueprintCallable)
	UAnimInstance* GetWeaponAnimInstance() const {return WeaponMeshComp->GetAnimInstance();}

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetWeaponSkeletalMeshComponent() const {return WeaponMeshComp;}

	

	int32 GetItemID() const {return ItemID;}
	

	bool IsEquippableInSlot(const FGameplayTag& InTargetSlot) const;

	
	const TArray<TSubclassOf<UBaseGameplayAbility>>& GetAbilitiesGrantByWeapon() const { return WeaponGrantAbilities; }


	AWeaponPartsBase* GetPartsInSlot(const FGameplayTag& InSlotTag, bool bGetDefault);

	UFUNCTION(BlueprintPure)
	UWeaponPartsComponent* GetWeaponPartsComponent() const {return WeaponPartsComponent;}

	
};

