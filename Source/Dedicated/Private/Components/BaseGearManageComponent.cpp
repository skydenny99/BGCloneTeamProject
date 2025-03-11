// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BaseGearManageComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "Gear/BaseGear.h"


// Sets default values for this component's properties
UBaseGearManageComponent::UBaseGearManageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UBaseGearManageComponent::EquipGear(ABaseGear* Gear)
{
	if (Gear == nullptr) return;
	UnEquipGear(Gear->GetGearType());
	EquippedGearMap[Gear->GetGearType()] = Gear;
	
	if (Gear->GetGearGameplayEffectClass())
	{
		if (UBaseAbilitySystemComponent* ASC = UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(GetOwner()))
		{
			ASC->ApplyGameplayEffectToSelf(
				Gear->GetGearGameplayEffectClass().GetDefaultObject(),
				Gear->GetGearLevel(),
				ASC->MakeEffectContext()
				);
		}
	}
	Gear->OnEquip();
}

void UBaseGearManageComponent::UnEquipGear(FGameplayTag GearType)
{
	if (ABaseGear* TargetGear = EquippedGearMap[GearType])
	{
		if (UBaseAbilitySystemComponent* ASC = UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(GetOwner()))
		{
			FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(GearType.GetSingleTagContainer());
			ASC->RemoveActiveEffects(Query);
		}
		TargetGear->OnUnEquip();
	}
	EquippedGearMap[GearType] = nullptr;
}

