// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/BaseWeaponFieldItem.h"
#include "BaseGunFieldItem.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API ABaseGunFieldItem : public ABaseWeaponFieldItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated, EditDefaultsOnly, Category="ItemInfo")
	int32 CurrentAmmo;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	virtual void InitWeaponSpec(AWeaponBase* Weapon) override;
	virtual void OverrideWeaopnSpec(AWeaponBase* Weapon) override;
	
	
};
