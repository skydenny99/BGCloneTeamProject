// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/PlayerStateSlot/BagSlotWidget.h"
#include "Components/ProgressBar.h"
#include "Math/UnrealMathUtility.h"
#include "Styling/SlateBrush.h"
#include "Engine/Texture2D.h"

void UBagSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetToDefaultState();
}

void UBagSlotWidget::UpdateBagGuage(float _Min, float _Max, float _CurrentBagWeight)
{
	if (!BagBar)
	{
		return;
	}
	
	// 게이지 값 계산
	CurrentBagBar=FMath::Clamp((_CurrentBagWeight - _Min) / (_Max - _Min),0.0f,1.0f);

	// ProgressBar 값 업데이트
	BagBar->SetPercent(CurrentBagBar);
}

void UBagSlotWidget::UpdateBagBarTexture(int32 _BagLevel)
{
	if (!BagBar)
	{
		return;
	}

	// 가방 레벨에 따라 텍스쳐 경로를 가져옴
	FString TexturePath = GetBagTexturePath(_BagLevel);

	// 텍스쳐 로드
	UTexture2D* BagTexture = LoadObject<UTexture2D>(nullptr, *TexturePath);
	
	if (BagTexture)
	{
		// ProgressBar의 기존 스타일 가져오기
		FProgressBarStyle NewStyle = BagBar->GetWidgetStyle();		
		
		// FillImage 설정
		FSlateBrush FillBrush;
		FillBrush.SetResourceObject(BagTexture);
		//BagBar->WidgetStyle.FillImage = FillBrush;
		NewStyle.SetFillImage(FillBrush);
		
		// BackGroundImage 설정(알파값 적용)
		FSlateBrush BackgroundBrush;
		BackgroundBrush.SetResourceObject(BagTexture);
		BackgroundBrush.TintColor = FSlateColor(FLinearColor(1.0f,1.0f,1.0f,0.3f));
		//BagBar->WidgetStyle.BackgroundImage = BackgroundBrush;
		NewStyle.SetBackgroundImage(BackgroundBrush);

		// ProgressBar에 새로운 스타일 적용
		BagBar->SetWidgetStyle(NewStyle);
	}	
}

FString UBagSlotWidget::GetBagTexturePath(int32 _BagLevel = 0) const
{
	switch (_BagLevel)
	{
	case 0:
		return TEXT("/Game/Asset/UI/HUD/Crosshair/Crosshairs_01_C.Crosshairs_01_C");
	case 1:
		return TEXT("/Game/Asset/UI/HUD/Equipment/equipment_bag01.equipment_bag01");
	case 2:
		return TEXT("/Game/Asset/UI/HUD/Equipment/equipment_bag02.equipment_bag02");
	case 3:
		return TEXT("/Game/Asset/UI/HUD/Equipment/equipment_bag03.equipment_bag03");
	default:
		return TEXT("/Game/Asset/UI/HUD/Crosshair/Crosshairs_01_C.Crosshairs_01_C");
	}
}

void UBagSlotWidget::SetToDefaultState()
{
	if (!BagBar)
	{
		return;
	}

	// 디폴트 텍스쳐 경로
	FString DefaultTexturePath = GetBagTexturePath();

	// 텍스쳐 로드
	UTexture2D* DefaultTexture = LoadObject<UTexture2D>(nullptr, *DefaultTexturePath);
	
	if (DefaultTexture)
	{
		// ProgressBar의 기존 스타일 가져오기
		FProgressBarStyle NewStyle = BagBar->GetWidgetStyle();
		
		// FillImage 설정
		FSlateBrush FillBrush;
		FillBrush.SetResourceObject(DefaultTexture);
		NewStyle.SetFillImage(FillBrush);
		
		// BackgroundImage 설정 (알파값 적용)
		FSlateBrush BackgroundBrush;
		BackgroundBrush.SetResourceObject(DefaultTexture);
		BackgroundBrush.TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
		NewStyle.SetBackgroundImage(BackgroundBrush);

		// ProgressBar에 새로운 스타일 적용
		BagBar->SetWidgetStyle(NewStyle);

		// ProgressBar를 디폴트 상태로 초기화
		BagBar->SetPercent(0.0f);
	}	
}
