// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/CompassWidget.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void UCompassWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	// 머티리얼 생성
	UMaterialInterface* ParentMaterial=LoadObject<UMaterialInterface>(nullptr,TEXT("/Game/_BP/UI/HUD_Child/Compass/Material/MAT_Compass.MAT_Compass"));

	if (ParentMaterial)
	{
		CompassMaterial=UMaterialInstanceDynamic::Create(ParentMaterial,this);
	}

	// 이미지에 머티리얼 적용
	if (CompassMaterial&&CompassImage)
	{
		CompassImage->SetBrushFromMaterial(CompassMaterial);
	}

	
}

void UCompassWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 0.01초마다 UpdateCompass 호출
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&UCompassWidget::UpdateCompass,0.01f,true);
	
}

void UCompassWidget::UpdateCompass()
{
	if (!CompassMaterial || !DirectionText)
	{
		return;
	}

	// 플레이어 컨트롤러 가져오기
	APlayerController* PlayerController=UGameplayStatics::GetPlayerController(GetWorld(),0);
	if (PlayerController)
	{
		// 플레이어 회전값 가져오기
		FRotator ControlRotation=PlayerController->GetControlRotation();
		float Yaw = FMath::Fmod(ControlRotation.Yaw, 360.0f);
		
		if (Yaw < 0.0f)
		{
			Yaw += 360.0f;
		}

		// 나침반 머티리얼 업데이트
		CompassMaterial->SetScalarParameterValue(ParameterName, Yaw / 360.0f);

		// 방향 텍스트 계산
		FString DirectionTextValue;
		FLinearColor DirectionColor = FLinearColor(0.8f,0.8f,0.8f,1.0);

		int32 RoundedYaw = FMath::RoundToInt(Yaw / 5.0f) * 5; // 5도 단위

		// 주요 방향 텍스트 설정
		switch (RoundedYaw)
		{
		case 0:
		case 360:
			DirectionTextValue=TEXT("N");
			DirectionColor=FLinearColor(1.0f,0.7f,0.0f,1.0);
			break;
		case 45:
			DirectionTextValue=TEXT("NE");
			break;
		case 90:
			DirectionTextValue=TEXT("E");
			break;
		case 135:
			DirectionTextValue=TEXT("SE");
			break;
		case 180:
			DirectionTextValue=TEXT("S");
			break;
		case 225:
			DirectionTextValue=TEXT("SW");
			break;
		case 270:
			DirectionTextValue=TEXT("W");
			break;
		case 315:
			DirectionTextValue=TEXT("NW");
			break;
		default:
			DirectionTextValue=FString::Printf(TEXT("%d"), RoundedYaw);
			break;
		}
		// 텍스트 및 색상 업데이트
		DirectionText->SetText(FText::FromString(DirectionTextValue));
		DirectionText->SetColorAndOpacity(FSlateColor(DirectionColor));		
	}
}