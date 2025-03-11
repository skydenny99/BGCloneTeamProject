// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Parts/WeaponPartsBase.h"
#include "WeaponMagazineBase.generated.h"

class ABaseGun;
/**
 * 
 */
UCLASS()
class DEDICATED_API AWeaponMagazineBase : public AWeaponPartsBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	uint8 AdditionalMaxAmmo = 0;

	UPROPERTY(EditDefaultsOnly)
	float ReloadSpeedRate = 1.f;
	
public:
	virtual void OnEquipParts(AActor* InWeapon) override;
};
