// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Parts/Magazine/WeaponMagazineBase.h"

#include "Weapon/BaseGun.h"


void AWeaponMagazineBase::OnEquipParts(AActor* InWeapon)
{
	Super::OnEquipParts(InWeapon);
	if (ABaseGun* Gun = Cast<ABaseGun>(InWeapon); IsValid(Gun))
	{
		Gun->IncreaseMaxAmmo(AdditionalMaxAmmo);
		Gun->SetReloadSpeedRate(ReloadSpeedRate);
	}
}
