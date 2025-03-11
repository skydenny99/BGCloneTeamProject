// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/PlayerStateSlot/HelmetSlotWidget.h"
#include "Components/ProgressBar.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Texture2D.h"


void UHelmetSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 디폴트 상태로 초기화
	SetToDefaultState();
}

void UHelmetSlotWidget::UpdateHelmetDurability(float _NewDurability)
{
	if (!HelmetBar)
	{
		return;
	}

	// 내구도 값 업데이트(0 ~ MaxDurability 사이로 클램프)
	CurrentDurability = FMath::Clamp(_NewDurability, 0.0f, MaxDurability);

	// 내구도 퍼센트 계산(1에서 0으로 감소하는 방식)
	//float DurabilityPercentage = (MaxDurability > 0.0f) ? (CurrentDurability / MaxDurability) : 0.0f;
	float DurabilityPercentage = 0.0f;
	if (MaxDurability > 0.0f)
	{
		DurabilityPercentage=CurrentDurability/MaxDurability;
	}
	
	// ProgressBar 업데이트
	HelmetBar->SetPercent(DurabilityPercentage);

	// 내구도가 0이면 ProgressBar를 빨간색으로 설정
	if (CurrentDurability <= 0.0f)
	{
		SetHelmetBarToCriticalState();
	}	
}

void UHelmetSlotWidget::UpdateHelmetBarTexture(int32 _NewHelmetLevel)
{
	if (!HelmetBar)
	{
		return;
	}

	// 헬멧 레벨 설정
	HelmetLevel=FMath::Clamp(_NewHelmetLevel,0,3);

	// 헬멧 레벨에 따라 텍스쳐 경로 가져오기
	FString TexturePath = GetHelmetTexturePath(HelmetLevel);

	// 텍스쳐 로드
	UTexture2D* HelmetTexture = LoadObject<UTexture2D>(nullptr, *TexturePath);

	if (HelmetTexture)
	{
		// ProgressBar의 기존 스타일 가져오기
		FProgressBarStyle NewStyle = HelmetBar->GetWidgetStyle();
		
		// Fill 이미지 설정
		FSlateBrush FillBrush;
		FillBrush.SetResourceObject(HelmetTexture);
		//HelmetBar->WidgetStyle.FillImage = FillBrush;
		NewStyle.SetFillImage(FillBrush);
		

		// Background 이미지 설정(알파값 적용)
		FSlateBrush BackgroundBrush;
		BackgroundBrush.SetResourceObject(HelmetTexture);
		BackgroundBrush.TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
		//HelmetBar->WidgetStyle.BackgroundImage=BackgroundBrush;
		NewStyle.SetBackgroundImage(BackgroundBrush);

		// ProgressBar에 새로운 스타일 적용
		HelmetBar->SetWidgetStyle(NewStyle);		
	}	
}

FString UHelmetSlotWidget::GetHelmetTexturePath(int32 _HelmetLevel = 0) const
{
	// 헬멧 레벨에 따른 텍스쳐 경로를 반환
	switch (_HelmetLevel)
	{
	case 0:
		return TEXT("/Game/Asset/UI/HUD/Crosshair/Crosshairs_01_C.Crosshairs_01_C");
	case 1:
		return TEXT("/Game/Asset/UI/HUD/Equipment/equipment_helmet01.equipment_helmet01");
	case 2:
		return TEXT("/Game/Asset/UI/HUD/Equipment/equipment_helmet02.equipment_helmet02");
	case 3:
		return TEXT("/Game/Asset/UI/HUD/Equipment/equipment_helmet03.equipment_helmet03");
	default:
		return TEXT("/Game/Asset/UI/HUD/Crosshair/Crosshairs_01_C.Crosshairs_01_C");
		
	}
}

void UHelmetSlotWidget::SetToDefaultState()
{
	if (!HelmetBar)
	{
		return;
	}

	// 디폴트 상태로 초기화
	CurrentDurability = 0.0f;
	MaxDurability = 100.0f;
	HelmetLevel = 0;
	
	// 디폴트 텍스쳐 경로 가져오기
	FString DefaultHelmetTexturePath = GetHelmetTexturePath();

	// 텍스쳐 로드
	UTexture2D* DefaultTexture = LoadObject<UTexture2D>(nullptr, *DefaultHelmetTexturePath);

	if (DefaultTexture)
	{
		// ProgressBar의 기존 스타일 가져오기
		FProgressBarStyle NewStyle=HelmetBar->GetWidgetStyle();
		
		// Fill 이미지 설정
		FSlateBrush FillBrush;
		FillBrush.SetResourceObject(DefaultTexture);
		// HelmetBar->WidgetStyle.FillImage=FillBrush;
		NewStyle.SetFillImage(FillBrush);

		// Background 이미지 설정(알파 값 적용)
		FSlateBrush BackgroundBrush;
		BackgroundBrush.SetResourceObject(DefaultTexture);
		BackgroundBrush.TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.3f);
		// HelmetBar->WidgetStyle.BackgroundImage=BackgroundBrush;
		NewStyle.SetBackgroundImage(BackgroundBrush);

		// ProgressBar에 새로운 스타일 적용
		HelmetBar->SetWidgetStyle(NewStyle);

		// ProgressBar 초기화(내구도 0%)
		HelmetBar->SetPercent(0.0f);
	}	
}

void UHelmetSlotWidget::SetHelmetBarToCriticalState()
{
	if (HelmetBar)
	{
		// ProgressBar를 빨강으로 설정
		FSlateBrush CriticalBrush;
		CriticalBrush.TintColor=FSlateColor(FLinearColor::Red);

		// ProgressBar 스타일 업데이트
		FProgressBarStyle NewStyle=HelmetBar->GetWidgetStyle();
		NewStyle.SetFillImage(CriticalBrush);
		HelmetBar->SetWidgetStyle(NewStyle);

		// Fill 이미지에 적용
		//HelmetBar->WidgetStyle.FillImage=CriticalBrush;		
	}
}
