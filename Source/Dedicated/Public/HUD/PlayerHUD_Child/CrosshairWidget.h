// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

/**
 * 
 */

class UBorder;

UCLASS()
class DEDICATED_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UBorder* TopCrosshair;

	UPROPERTY(meta = (BindWidget))
	UBorder* BottomCrosshair;

	UPROPERTY(meta = (BindWidget))
	UBorder* LeftCrosshair;

	UPROPERTY(meta = (BindWidget))
	UBorder* RightCrosshair;

public:
	// 크로스헤어 속성
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Crosshair")
	float CrosshairThickness=4.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Crosshair")
	float CrosshairLength= 12.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
	float CrosshairSpread = 10.0f;

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
