// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "WeaponPartsBase.generated.h"

class AWeaponBase;

UCLASS()
class DEDICATED_API AWeaponPartsBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponPartsBase();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Item Info")
	int32 ItemID;
	
	UPROPERTY(EditDefaultsOnly, Category="Item Info", meta=(Categories="Item.Parts"))
	FGameplayTag SlotTag;

public:
	virtual void OnEquipParts(AActor* InWeapon) {}
	virtual void OnUnequipParts(AActor* InWeapon) {}
	bool IsEquippable(const FGameplayTagContainer& InPartsSlotContainer) const;
	FORCEINLINE const FGameplayTag& GetEquipSlotTag() const { return SlotTag; }
	FORCEINLINE FGameplayTag GetEquipSlotTagName() { return SlotTag; }
	FORCEINLINE int32 GetItemID() const { return ItemID; }
};
