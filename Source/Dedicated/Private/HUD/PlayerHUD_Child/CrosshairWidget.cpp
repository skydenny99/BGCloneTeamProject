// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/CrosshairWidget.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"


void UCrosshairWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// BottomCrosshair
	if (BottomCrosshair)
	{
		UCanvasPanelSlot* BottomSlot=Cast<UCanvasPanelSlot>(BottomCrosshair->Slot);
		if (BottomSlot)
		{
			BottomSlot->SetSize(FVector2D(CrosshairThickness,CrosshairLength));
			BottomSlot->SetPosition(FVector2D(-(CrosshairThickness / 2.0f),CrosshairSpread));
		}
	}

	// TopCrosshair
	if (TopCrosshair)
	{
		UCanvasPanelSlot* TopSlot=Cast<UCanvasPanelSlot>(TopCrosshair->Slot);
		if (TopSlot)
		{
			TopSlot->SetSize(FVector2D(CrosshairThickness,CrosshairLength));
			TopSlot->SetPosition(FVector2D(-(CrosshairThickness / 2.0f),-CrosshairSpread-CrosshairLength));
		}
	}

	// LeftCrosshair
	if (LeftCrosshair)
	{
		UCanvasPanelSlot* LeftSlot=Cast<UCanvasPanelSlot>(LeftCrosshair->Slot);
		if (LeftSlot)
		{
			LeftSlot->SetSize(FVector2D(CrosshairLength,CrosshairThickness));
			LeftSlot->SetPosition(FVector2D(-CrosshairSpread-CrosshairLength,-(CrosshairThickness / 2.0f)));
		}
	}

	// RightCrosshair
	if (RightCrosshair)
	{
		UCanvasPanelSlot* RightSlot=Cast<UCanvasPanelSlot>(RightCrosshair->Slot);
		if (RightSlot)
		{
			RightSlot->SetSize(FVector2D(CrosshairLength,CrosshairThickness));
			RightSlot->SetPosition(FVector2D(CrosshairSpread,-(CrosshairThickness / 2.0f)));			
		}
	}	
}
