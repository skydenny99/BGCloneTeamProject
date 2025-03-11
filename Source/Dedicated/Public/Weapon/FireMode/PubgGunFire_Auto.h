// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/PubgGunFireBase.h"
#include "PubgGunFire_Auto.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API UPubgGunFire_Auto : public UPubgGunFireBase
{
	GENERATED_BODY()

protected:
	float FireRate;
	FTimerHandle RefireTimer;

	UFUNCTION()
	void CheckRefire();
	
public:
	//virtual void InitGun(ABaseGun* InGun) override;
	virtual void BeginFire() override;
	virtual void EndFire() override;
	virtual void OnPutAway() override;
	
};
