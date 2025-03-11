// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/MiniMapWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Components/Image.h"


void UMiniMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 플레이어 캐릭터 가져오기
	APlayerController* PlayerController=UGameplayStatics::GetPlayerController(this,0);
	if (PlayerController)
	{
		APawn* PlayerPawn = PlayerController->GetPawn();
		if (PlayerPawn)
		{
			// 플레이어 위치와 회전 가져오기
			FVector PlayerLocation = PlayerPawn->GetActorLocation();
			FRotator PlayerRotation = PlayerController->PlayerCameraManager->GetCameraRotation();

			// 머티리얼 파라미터 컬렉션 인스턴스 가져오기
			if (MiniMapMPC)
			{
				UMaterialParameterCollectionInstance* CollectionInstance=GetWorld()->GetParameterCollectionInstance(MiniMapMPC);

				if (CollectionInstance)
				{
					// X, Y 위치 업데이트
					CollectionInstance->SetScalarParameterValue(FName("X"), PlayerLocation.X);
					CollectionInstance->SetScalarParameterValue(FName("Y"), PlayerLocation.Y);
				}
			}

			// 화살표 회전 업데이트
			if (MiniMapArrow)
			{
				float MiniMapArrowAngle=(PlayerRotation.Yaw -= 45.f) * 1.0f; // Yaw 값을 음수로 변환
				MiniMapArrow->SetRenderTransformAngle(MiniMapArrowAngle);				
			}
			// FOV 회전 업데이트
			if (MiniMapFOV)
			{
				float MiniMapFOVAngle=(PlayerRotation.Yaw -= 45.f)* 1.0f; // Yaw 값을 음수로 변환
				MiniMapFOV->SetRenderTransformAngle(MiniMapFOVAngle);				
			}
		}
	}	
}
