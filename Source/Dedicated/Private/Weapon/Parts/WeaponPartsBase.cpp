// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Parts/WeaponPartsBase.h"


// Sets default values
AWeaponPartsBase::AWeaponPartsBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

bool AWeaponPartsBase::IsEquippable(const FGameplayTagContainer& InPartsSlotContainer) const
{
	return SlotTag.MatchesAny(InPartsSlotContainer);
}
