// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/BaseGunFieldItem.h"

#include "BaseDebugHelper.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/BaseGun.h"


void ABaseGunFieldItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseGunFieldItem, CurrentAmmo);
}

void ABaseGunFieldItem::InitWeaponSpec(AWeaponBase* Weapon)
{
	if (IsValid(Weapon) == false) return;
	Super::InitWeaponSpec(Weapon);
	if (ABaseGun* Gun = Cast<ABaseGun>(Weapon))
	{
		CurrentAmmo = Gun->GetCurrentAmmo();
	}
}

void ABaseGunFieldItem::OverrideWeaopnSpec(AWeaponBase* Weapon)
{
	if (IsValid(Weapon) == false) return;
	Super::OverrideWeaopnSpec(Weapon);
	if (ABaseGun* Gun = Cast<ABaseGun>(Weapon))
	{
		Gun->SetCurrentAmmo(CurrentAmmo);
	}
}
