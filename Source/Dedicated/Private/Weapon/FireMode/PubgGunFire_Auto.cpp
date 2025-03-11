// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/FireMode/PubgGunFire_Auto.h"

#include "Weapon/BaseGun.h"


void UPubgGunFire_Auto::CheckRefire()
{
}

// void UPubgGunFire_Auto::InitGun(ABaseGun* InGun)
// {
// 	Super::InitGun(InGun);
// 	if (IsValid(InGun) == false) return;
// 	const FGunSpec& GunSpec = InGun->GetGunSpec(); 
// 	FireRate = GunSpec.FireRate[BaseGameplayTags::Weapon_Gun_FireMode_Auto];
// }

void UPubgGunFire_Auto::BeginFire()
{
	if (OwningGun.IsValid() == false) return;
	OwningGun->GetWorldTimerManager().SetTimer(RefireTimer, this, &UPubgGunFire_Auto::CheckRefire, FireRate, false);

}

void UPubgGunFire_Auto::EndFire()
{
	if (OwningGun.IsValid() == false) return;
	OwningGun->GetWorldTimerManager().ClearTimer(RefireTimer);
}

void UPubgGunFire_Auto::OnPutAway()
{
	if (OwningGun.IsValid() == false) return;
	OwningGun->GetWorldTimerManager().ClearTimer(RefireTimer);
}
