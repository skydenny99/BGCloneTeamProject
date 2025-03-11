// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FreeFallingWidget.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class DEDICATED_API UFreeFallingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetDistance(const float _LocationZ);
	float GetDistance();


private:
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Distance;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Distance;

	/** 지상까지의 거리를 담고 있는 변수 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float DistanceToGround = 9999.f;

	/** 비행기 스폰 높이 */
	static constexpr float PlaneHeight = 80000.0f;

	/** 땅 높이 */
	static constexpr float GroundHeight = 100.0f;

	/** FreeFalling 위젯 사이즈 */
	static constexpr float WidgetSize = 1650.0f;

	
	
	
};
