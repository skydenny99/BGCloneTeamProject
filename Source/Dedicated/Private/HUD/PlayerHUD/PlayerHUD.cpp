// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD/PlayerHUD.h"

#include "Components/Image.h"
#include "Components/VerticalBox.h"
//#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"

//#include "Characters/PubgCharacter.h"
#include "Characters/PubgCharacter.h"
#include "Components/BaseWeaponManageComponent.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBoxSlot.h"
#include "HUD/PlayerHUD_Child/FreeFallingWidget.h"
#include "HUD/PlayerHUD_Child/Interact.h"

#include "HUD/PlayerHUD_Child/PlayerStateWidget.h"
#include "Slate/SGameLayerManager.h"
//#include "Weapon/BaseGun.h"

void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 현재 총에 장착된 총알 감소 델리게이트
	//OnFireCurrentAmmo.BindUFunction(this, FName("OnFireCurrentAmmo"));
	OnFireCurrentAmmo.BindUObject(this,&UPlayerHUD::UpdateCurrentDecreaseAmmo);

	// 킬 피드 델리게이트
	OnKillFeed.BindUObject(this,&UPlayerHUD::UpdateKillFeed);

	// 킬 카운터 델리게이트
	OnKillCount.BindUObject(this,&UPlayerHUD::UpdateKillCount);

	// 아이콘 매핑 초기화
	InitializeWeaponIcon();

	bActiveFreeFallingWidget = false;
}

void UPlayerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bActiveFreeFallingWidget) 
	{
		if (APubgCharacter* PlayerPawn = Cast<APubgCharacter>(GetOwningPlayerPawn()))
		{
			WBP_FreeFalling->SetDistance(PlayerPawn->GetActorLocation().Z);
		}
		
		const float TempDistanceToGround =GetDistanceToGround();
		
		if (TempDistanceToGround <= 100.f && TempDistanceToGround >= 95.f)
		{
			if (!WBP_Interact->IsVisible())
			{
				ToggleInteractWidget(true);
				WBP_Interact->SetDestText(FText::FromString(TEXT("낙하산 펴기")),false);
				SetInteractWidgetSize();
			}
		}

		if ((TempDistanceToGround <= 30.f && TempDistanceToGround >= 28.f) || (TempDistanceToGround <= 15.f && TempDistanceToGround >= 12.f))
		{
			if (!WBP_Interact->IsVisible())
			{
				ToggleInteractWidget(true);
				WBP_Interact->SetDestText(FText::FromString(TEXT("낙하산 접기")),false);
				SetInteractWidgetSize();
			}
			
			
		}
	}
}

void UPlayerHUD::TriggerFreeFallingWidget(const bool _Active)
{
	
	if (_Active)
	{
		bActiveFreeFallingWidget = true;
		WBP_FreeFalling->SetVisibility(ESlateVisibility::HitTestInvisible);
		Debug::Print("FreeFallingWidget Active");
	}
	else
	{
		bActiveFreeFallingWidget = false;
		WBP_FreeFalling->SetVisibility(ESlateVisibility::Hidden);
		Debug::Print("FreeFallingWidget DeActive");
	}
}

void UPlayerHUD::UpdateHealth(const float NewCurrentHp, const float NewMaxHp)
{
	// PlayerState 위젯이 유효한지 확인
	if (WBP_PlayerState)
	{
		// PlayerState위젯의 UpdateHealth 함수 호출
		WBP_PlayerState->UpdateHealth(NewCurrentHp, NewMaxHp);
	}
}

void UPlayerHUD::UpdateAmmoOverlay(EGunType _GunType)
{
	if (WBP_PlayerState)
	{
		WBP_PlayerState->UpdateAmmoOverlay(_GunType);
	}
}

void UPlayerHUD::UpdateFireMode(const FGameplayTag& _FireMode)
{
	if (WBP_PlayerState)
	{
		WBP_PlayerState->UpdateFireMode(_FireMode);
	}
}

void UPlayerHUD::UpdateCurrentDecreaseAmmo(int32 _CurrentAmmoAfterFire)
{
	if (WBP_PlayerState)
	{
		WBP_PlayerState->UpdateCurrentDecreaseAmmo(_CurrentAmmoAfterFire);
	}
}

void UPlayerHUD::SetHUDCurrentAmmo(int32 Val)
{
	if (WBP_PlayerState)
	{
		WBP_PlayerState->SetCurrentAmmo(Val);
	}
}

void UPlayerHUD::UpdatePossessedAmmo(int32 _PossessedAmmo)
{
	if (WBP_PlayerState)
	{
		WBP_PlayerState->UpdatePossessedAmmo(_PossessedAmmo);
	}
}

void UPlayerHUD::UpdateAmmoSlot(const bool _bTrigger, const EPlayerActionMode _PlayerActionMode,
	const EGunType _GunType, const FGameplayTag& _FireMode)
{
	if (WBP_PlayerState)
	{
		WBP_PlayerState->UpdateAmmoSlot(_bTrigger, _PlayerActionMode, _GunType, _FireMode);
	}
}

void UPlayerHUD::UpdateKillCount(int32 _KillCount)
{
	if (KillCount && HOR_KillCountBox && KillCountVisible)
	{
		KillCount->SetText(FText::AsNumber(_KillCount));

		// HOR_KillCountBox 표시
		HOR_KillCountBox->SetVisibility(ESlateVisibility::HitTestInvisible);

		// KillCountVisible 애니메이션 실행
		PlayAnimation(KillCountVisible);

		// 5초 후 다시 숨기기
		FTimerHandle HideTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			HideTimerHandle,
			FTimerDelegate::CreateLambda([this]()
			{
				if (HOR_KillCountBox)
				{
					HOR_KillCountBox->SetVisibility(ESlateVisibility::Hidden);
				}
			}),
			5.0f,
			false
			);		
	}
}

void UPlayerHUD::UpdateKillFeed(const FString& _Player, const FString& _Enemy, int32 _GunID)
{
	if (!VER_KillFeedBox)
	{
		return;
	}

	if (!KillFeedSlotClass)
	{
		Debug::Print("Please Setup WBP KillFeedSlotClass");
		return;
	}

	// 1. KillFeedWidget 생성
	UKillFeedWidget* NewKillFeedWidget = CreateWidget<UKillFeedWidget>(this, KillFeedSlotClass);
	if (!NewKillFeedWidget)
	{
		return;
	}
	
	// 2. KillFeedWidget 내용 설정
	if (NewKillFeedWidget)
	{
		NewKillFeedWidget->SetPlayerText(FText::FromString(_Player));
		NewKillFeedWidget->SetEnemyText(FText::FromString(_Enemy));
	}

	// 3. WeaponIconMap에서 아이콘 가져오기
	UTexture2D* WeaponIcon = nullptr;
	if (WeaponIconMap.Contains(_GunID))
	{
		WeaponIcon = WeaponIconMap[_GunID];
	}
	
	if (NewKillFeedWidget && WeaponIcon)
	{
		NewKillFeedWidget->SetWeaponIcon(WeaponIcon);
	}
	
	// 4. VerticalBox의 맨 위에 추가
	UVerticalBoxSlot* NewSlot = VER_KillFeedBox->AddChildToVerticalBox(NewKillFeedWidget);

	if (NewSlot)
		NewSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Right);
	
	// 애니메이션 재생
	NewKillFeedWidget->PlayKillAnimation();

	// 5. KillFeedBox 항목 수 제한
	if (VER_KillFeedBox->GetChildrenCount() > 5)
	{
		UWidget* OldestWidget = VER_KillFeedBox->GetChildAt(0);
		VER_KillFeedBox->RemoveChild(OldestWidget);
	}

	// 6. 일정 시간 후 자동 제거
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		FTimerDelegate::CreateLambda([this, NewKillFeedWidget]()
		{
			if (VER_KillFeedBox && NewKillFeedWidget)
			{
				VER_KillFeedBox->RemoveChild(NewKillFeedWidget);
			}
		}),
		5.0f,
		false
		);	
}

void UPlayerHUD::InitializeWeaponIcon()
{
	// 아이콘 로드
	/*UTexture2D* AKIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/_BP/UI/WeaponIcon/Weapon_AK47.Weapon_AK47"));
	UTexture2D* PistolIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/_BP/UI/WeaponIcon/Weapon_GLOCK.Weapon_GLOCK"));
	UTexture2D* ShotGunIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/_BP/UI/WeaponIcon/Weapon_S1897.Weapon_S1897"));
	UTexture2D* SniferIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/_BP/UI/WeaponIcon/Weapon_M24.Weapon_M24"));
	UTexture2D* SMGIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/_BP/UI/WeaponIcon/Weapon_UZI.Weapon_UZI"));*/

	// 맵에 추가
	/*WeaponIconMap.Add(EGunType::AR, ARIcon);
	WeaponIconMap.Add(EGunType::LMG, ARIcon);
	WeaponIconMap.Add(EGunType::Pistol, PistolIcon);
	WeaponIconMap.Add(EGunType::SG, ShotGunIcon);
	WeaponIconMap.Add(EGunType::SR, SniferIcon);
	WeaponIconMap.Add(EGunType::DMR, SniferIcon);
	WeaponIconMap.Add(EGunType::SMG, SMGIcon);	*/
}

void UPlayerHUD::ToggleInteractWidget(const bool Active)
{
	if (Active)
	{
		WBP_Interact->SetVisibility(ESlateVisibility::HitTestInvisible);
		Debug::Print("Interact Widget :: Active");
	}
	else
	{
		WBP_Interact->SetVisibility(ESlateVisibility::Collapsed);
		Debug::Print("Interact Widget :: DeActive");
	}

}

void UPlayerHUD::SetInteractWidgetSize()
{
	float Length = GetInteractWidget()->GetLengthDescText() * 15 + 30; // 15 = FontSize, 30 = 줍기 포함

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(GetInteractWidget()->Slot))
	{
		CanvasSlot->SetSize(FVector2D(Length, 40.f));
	}
}

void UPlayerHUD::UpdatePossessWeapon()
{
	WBP_PossessedItem->UpdateWeapon();
}
