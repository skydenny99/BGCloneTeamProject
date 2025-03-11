// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseType/BaseEnumType.h"
#include "BaseGameplayTags.h"
#include "AmmoSlotWidget.generated.h"

class UImage;
class UOverlay;

/**
 * 
 */
UCLASS()
class DEDICATED_API UAmmoSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// AR Overlay
	UPROPERTY(meta = (BindWidget))
	UOverlay* AR_Overlay;
	
	UPROPERTY(meta = (BindWidget))
	UImage* IMG_Ammo_1;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_Ammo_2;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_Ammo_3;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_Ammo_4;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_Ammo_5;

	// SR Overlay
	UPROPERTY(meta = (BindWidget))
	UOverlay* SR_Overlay;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_SR_Ammo;

	// SG Overlay
	UPROPERTY(meta = (BindWidget))
	UOverlay* SG_Overlay;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_SG_Ammo;

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category="Ammo Slot")
	void UpdateAmmoOverlay(EGunType _GunType);

	UFUNCTION(BlueprintCallable, Category="Ammo Slot")
	void UpdateFireMode(const FGameplayTag& _FireMode);

	UFUNCTION(BlueprintCallable, Category="Ammo Slot")
	void DeactivateFireMode();
	
	
	
};
