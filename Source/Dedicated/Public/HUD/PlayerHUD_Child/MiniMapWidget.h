// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapWidget.generated.h"

class UImage;
class UMaterialParameterCollection;

/**
 * 
 */
UCLASS()
class DEDICATED_API UMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// NativeTick을 오버라이드합니다.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(meta = (BindWidget))
	UImage* MiniMapArrow;

	UPROPERTY(meta = (BindWidget))
	UImage* MiniMapFOV;

	// 머티리얼 파라미터 컬렉션 참조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MiniMap")
	UMaterialParameterCollection* MiniMapMPC;
	
	
	
};
