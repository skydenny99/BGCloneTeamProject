// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Item/BaseFieldItem.h"
#include "BaseGearFieldItem.generated.h"

class ABaseGear;
/**
 * 
 */
UCLASS()
class DEDICATED_API ABaseGearFieldItem : public ABaseFieldItem
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="ItemInfo")
	TSubclassOf<ABaseGear> GearClass;

	UPROPERTY(EditDefaultsOnly, Category="ItemInfo")
	float CurrentDurability;

	UPROPERTY(EditDefaultsOnly, Category="ItemInfo")
	float MaxDurability;

	UPROPERTY(EditDefaultsOnly, Category="ItemInfo")
	FGameplayTag EquippableSlotTag;

public:
	TSubclassOf<ABaseGear> GetGearClass() const { return GearClass; }
	void SetDurability(float InDurability, float InMaxDurability) { CurrentDurability = InDurability; MaxDurability = InMaxDurability; }
	const FGameplayTag& GetEquippableSlot() const {return EquippableSlotTag;}
	FORCEINLINE float GetCurrentDurability() const { return CurrentDurability; }
	FORCEINLINE float GetMaxDurability() const { return MaxDurability; }
};
