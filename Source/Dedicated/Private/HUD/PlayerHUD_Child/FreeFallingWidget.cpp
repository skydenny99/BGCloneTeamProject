// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/FreeFallingWidget.h"

#include "BaseDebugHelper.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UFreeFallingWidget::SetDistance(const float _LocationZ)
{
	// 고도 계산
	float NormalizeHeight = FMath::Clamp((_LocationZ - GroundHeight) / (PlaneHeight - GroundHeight), 0.f, 1.f);

	// 매핑
	float WidgetY = FMath::Lerp(WidgetSize, 0.f, NormalizeHeight);
	DistanceToGround = (_LocationZ - GroundHeight) / 100.f;
	

	FVector2D NewLocation = FVector2D(0.f,WidgetY);
	Img_Distance->SetRenderTranslation(NewLocation);
	Text_Distance->SetRenderTranslation(NewLocation);
	Text_Distance->SetText(FText::FromString(FString::Printf(TEXT("%.0f M"),DistanceToGround)));
}

float UFreeFallingWidget::GetDistance()
{
	return DistanceToGround;
}

