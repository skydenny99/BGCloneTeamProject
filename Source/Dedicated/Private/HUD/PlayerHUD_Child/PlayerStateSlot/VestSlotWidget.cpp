// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/PlayerStateSlot/VestSlotWidget.h"
#include "Components/ProgressBar.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Texture2D.h"

void UVestSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 디폴트 상태로 초기화
	SetToDefaultState();
}

void UVestSlotWidget::UpdateVestDurability(float _NewDurability)
{
	if (!VestBar)
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
		DurabilityPercentage = CurrentDurability / MaxDurability;
	}

	// ProgressBar 업데이트
	VestBar->SetPercent(DurabilityPercentage);

	// 내구도가 0이면 ProgressBar를 빨간색으로 설정
	if (CurrentDurability <= 0.0f)
	{
		SetVestBarToCriticalState();
	}	
}

void UVestSlotWidget::UpdateVestBarTexture(int32 _NewVestLevel)
{
	if (!VestBar)
	{
		return;
	}

	// 조끼 레벨 설정
	VestLevel = FMath::Clamp(_NewVestLevel, 0, 3);

	// 조끼 레벨에 따라 텍스쳐 경로 가져오기
	FString TexturePath=GetVestTexturePath(VestLevel);

	// 텍스쳐 로드
	UTexture2D* VestTexture = LoadObject<UTexture2D>(nullptr, *TexturePath);

	if (VestTexture)
	{
		// ProgressBar의 기존 스타일 가져오기
		FProgressBarStyle NewStyle = VestBar->GetWidgetStyle();
		
		// Fill 이미지 설정
		FSlateBrush FillBrush;
		FillBrush.SetResourceObject(VestTexture);
		//VestBar->WidgetStyle.FillImage=FillBrush;
		NewStyle.SetFillImage(FillBrush);

		// Background 이미지 설정(알파값 적용)
		FSlateBrush BackgroundBrush;
		BackgroundBrush.SetResourceObject(VestTexture);
		BackgroundBrush.TintColor=FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
		//VestBar->WidgetStyle.BackgroundImage=BackgroundBrush;
		NewStyle.SetBackgroundImage(BackgroundBrush);

		// ProgressBar에 새로운 스타일 적용
		VestBar->SetWidgetStyle(NewStyle);
	}	
}

FString UVestSlotWidget::GetVestTexturePath(int32 _NewVestLevel = 0) const
{
	// 조끼 레벨에 따른 텍스쳐 경로를 반환
	switch (_NewVestLevel)
	{
	case 0:
		return TEXT("/Game/Asset/UI/HUD/Crosshair/Crosshairs_01_C.Crosshairs_01_C");
	case 1:
		return TEXT("/Game/Asset/UI/HUD/Equipment/equipment_vest01.equipment_vest01");
	case 2:
		return TEXT("/Game/Asset/UI/HUD/Equipment/equipment_vest02.equipment_vest02");
	case 3:
		return TEXT("/Game/Asset/UI/HUD/Equipment/equipment_vest03.equipment_vest03");
	default:
		return TEXT("/Game/Asset/UI/HUD/Crosshair/Crosshairs_01_C.Crosshairs_01_C");
	}
}


void UVestSlotWidget::SetToDefaultState()
{
	if (!VestBar)
	{
		return;
	}
	
	// 디폴트 상태로 초기화
	CurrentDurability = 0.0f;
	MaxDurability = 100.0f;
	VestLevel = 0;

	// 디폴트 텍스쳐 경로 가져오기
	FString DefaultTexturePath=GetVestTexturePath();

	// 텍스쳐 로드
	UTexture2D* DefaultTexture = LoadObject<UTexture2D>(nullptr, *DefaultTexturePath);

	if (DefaultTexture)
	{
		// ProgressBar의 기존 스타일 가져오기
		FProgressBarStyle NewStyle = VestBar->GetWidgetStyle();
		
		// Fill 이미지 설정
		FSlateBrush FillBrush;
		FillBrush.SetResourceObject(DefaultTexture);
		//VestBar->WidgetStyle.BackgroundImage=FillBrush;
		NewStyle.SetFillImage(FillBrush);

		// Background 이미지 설정(알파값 적용)
		FSlateBrush BackgroundBrush;
		BackgroundBrush.SetResourceObject(DefaultTexture);
		BackgroundBrush.TintColor=FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
		//VestBar->WidgetStyle.BackgroundImage=BackgroundBrush;
		NewStyle.SetBackgroundImage(BackgroundBrush);

		// ProgressBar에 새로운 스타일 적용
		VestBar->SetWidgetStyle(NewStyle);

		// ProgressBar 초기화(내구도 0%)
		VestBar->SetPercent(0.0f);		
	}	
}

void UVestSlotWidget::SetVestBarToCriticalState()
{
	if (VestBar)
	{
		// ProgressBar를 빨강으로 설정
		FSlateBrush CriticalBrush;
		CriticalBrush.TintColor=FSlateColor(FLinearColor::Red);

		// ProgressBar 스타일 업데이트
		FProgressBarStyle NewStyle = VestBar->GetWidgetStyle();
		NewStyle.SetFillImage(CriticalBrush);
		VestBar->SetWidgetStyle(NewStyle);

		// Fill 이미지에 적용
		//VestBar->WidgetStyle.FillImage=CriticalBrush;
	}
}