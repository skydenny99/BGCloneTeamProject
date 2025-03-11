// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayTags.h"
#include "Components/ActorComponent.h"
#include "BaseGearManageComponent.generated.h"


class ABaseGear;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEDICATED_API UBaseGearManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseGearManageComponent();
	
protected:
	UPROPERTY()
	TMap<FGameplayTag, class ABaseGear*> EquippedGearMap =
		{
		{BaseGameplayTags::Item_Gear_Helmet, nullptr},
		{BaseGameplayTags::Item_Gear_Armor, nullptr},
		{BaseGameplayTags::Item_Gear_Backpack, nullptr}
		};


public:
	void EquipGear(ABaseGear* Gear);
	void UnEquipGear(FGameplayTag GearType);
	ABaseGear* GetGear(FGameplayTag GearType) const { return EquippedGearMap[GearType]; }
		
	
};
