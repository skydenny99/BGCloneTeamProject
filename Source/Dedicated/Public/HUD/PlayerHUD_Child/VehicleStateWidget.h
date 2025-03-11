// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VehicleStateWidget.generated.h"

class UCarSlotWidget;
class UMotorCycleSlotWidget;
class UOilBarWidget;
class UProgressBar;


/**
 * 
 */
UCLASS()
class DEDICATED_API UVehicleStateWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UCarSlotWidget* WBP_CarSlot;

	UPROPERTY(meta = (BindWidget))
	UMotorCycleSlotWidget* WBP_MotorCycleSlot;

	UPROPERTY(meta = (BindWidget))
	UOilBarWidget* WBP_OilBar;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* VehicleHPBar;

public:
	FORCEINLINE UCarSlotWidget* GetCarSlotWidget(){return WBP_CarSlot;}
	FORCEINLINE UMotorCycleSlotWidget* GetMotorCycleSlotWidget(){return WBP_MotorCycleSlot;}
	FORCEINLINE UOilBarWidget* GetOilBarWidget(){return WBP_OilBar;}
	
	
};