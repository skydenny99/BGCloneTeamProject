// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayTags.h"
#include "GameplayTagContainer.h"
#include "Item/BaseFieldItem.h"
#include "BaseWeaponFieldItem.generated.h"

class AWeaponBase;
/**
 * 
 */
UCLASS()
class DEDICATED_API ABaseWeaponFieldItem : public ABaseFieldItem
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="ItemInfo")
	TSubclassOf<AWeaponBase> WeaponClass;
	UPROPERTY(EditDefaultsOnly, Category="ItemInfo", meta=(Categories = "Item.Slot"))
	FGameplayTagContainer EquippableSlotTags;

public:
	TSubclassOf<AWeaponBase> GetWeaponClass() const { return WeaponClass; }
	const FGameplayTagContainer& GetEquippableTags() const { return EquippableSlotTags; }
	virtual void InitWeaponSpec(AWeaponBase* Weapon);
	virtual void OverrideWeaopnSpec(AWeaponBase* Weapon);
};
