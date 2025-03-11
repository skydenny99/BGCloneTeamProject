// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Parts/WeaponPartsBase.h"
#include "WeaponSightBase.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API AWeaponSightBase : public AWeaponPartsBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly)
	FString SightSocketName;

	UPROPERTY(EditDefaultsOnly)
	float SightFov;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* ZoomSightMaterial;
	
public:
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnFPPZoomIN();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnFPPZoomOut();
	
	FORCEINLINE const FString& GetSocketName() const { return SightSocketName; }
	FORCEINLINE float GetFov() const { return SightFov; }

	
};
