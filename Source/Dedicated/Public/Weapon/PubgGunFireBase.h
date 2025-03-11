// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PubgGunFireBase.generated.h"

class ABaseGun;
/**
 * 
 */
UCLASS()
class DEDICATED_API UPubgGunFireBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	TWeakObjectPtr<class ABaseGun> OwningGun;
	
public:
	//virtual void InitGun(ABaseGun* InGun) { OwningGun = TWeakObjectPtr<ABaseGun>(InGun); }
	virtual void BeginFire() {}
	virtual void EndFire() {}
	virtual void OnPutAway() {}
};
