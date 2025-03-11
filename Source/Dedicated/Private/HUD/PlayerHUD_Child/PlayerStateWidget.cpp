// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/PlayerStateWidget.h"
#include "HUD/PlayerHUD_Child/PlayerStateSlot/AmmoSlotWidget.h"
//#include "HUD/PlayerHUD_Child/PlayerStateSlot/BagSlotWidget.h"
//#include "HUD/PlayerHUD_Child/PlayerStateSlot/HelmetSlotWidget.h"
//#include "HUD/PlayerHUD_Child/PlayerStateSlot/VestSlotWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"

#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"
#include "Components/Overlay.h"
#include "Engine/World.h"

void UPlayerStateWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 초기화
	if (HPBar)
	{
		HPBar->SetPercent(1.0f);
	}
	if (DamageIndicatorBar)
	{
		DamageIndicatorBar->SetPercent(1.0f);
	}
	if (BoostBar)
	{
		BoostBar->SetPercent(0.0f);
		BoostBar->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPlayerStateWidget::DecreaseBoostOverTime()
{
	if (!BoostBar || !IMG_BoostHeal)
	{
		return;
	}
	
	float DecreaseRate = 0.0f;
	
	if (CurrentBoost > 60.0f)
	{
		DecreaseRate = 0.8f;
	}
	else if (CurrentBoost > 40.0f)
	{
		DecreaseRate = 0.5f;
	}
	else
	{
		DecreaseRate = 0.2f;
	}
	
	// BoostBar 감소
	CurrentBoost = FMath::Clamp(CurrentBoost - DecreaseRate, 0.0f, MaxBoost);
	BoostBar->SetPercent(CurrentBoost / MaxBoost);

	// BoostBar가 0이 되면 타이머 종료
	if (CurrentBoost <=	0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(BoostTimerHandle);
		BoostBar->SetVisibility(ESlateVisibility::Collapsed);

		// BoostHeal 애니메이션 중지
		StopBoostHealPulse();
	}
	else
	{
		// BoostSpeedPulse 애니메이션 조건 체크
		if (CurrentBoost / MaxBoost >= 0.6f)
		{
			StartBoostSpeedPulse();
		}
		else
		{
			StopBoostSpeedPulse();
		}
	}
}

void UPlayerStateWidget::SetBoost(float NewCurrentBoost, float NewMaxBoost)
{
	if (!BoostBar || !IMG_BoostHeal || !IMG_BoostSpeed)
	{
		return;
	}

	// 외부에서 받은 값을 설정
	CurrentBoost = FMath::Clamp(NewCurrentBoost, 0.0f, NewMaxBoost);
	MaxBoost = FMath::Max(NewMaxBoost, 1.0f);

	// BoostBar 값 설정
	BoostBar->SetPercent(CurrentBoost / MaxBoost);
	
	// BoostBar가 숨겨져 있으면 표시
	if (CurrentBoost > 0.0f)
	{
		if (BoostBar->GetVisibility() == ESlateVisibility::Collapsed)
		{
			BoostBar->SetVisibility(ESlateVisibility::HitTestInvisible);

			// BoostHeal 애니메이션 시작
			StartBoostHealPulse();
		}

		// BoostSpeedPulse 조건 확인
		if (CurrentBoost / MaxBoost >= 0.6f)
		{
			StartBoostSpeedPulse();
		}
		else
		{
			StopBoostSpeedPulse();
		}

		// Boost 감소 타이머 시작
		if (!GetWorld()->GetTimerManager().IsTimerActive(BoostTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(
				BoostTimerHandle,
				this,
				&UPlayerStateWidget::DecreaseBoostOverTime,
				0.1f,
				true
				);
		}	
	}
	else
	{
		// Boost가 0일 경우 바로 Collapsed 처리
		BoostBar->SetVisibility(ESlateVisibility::Collapsed);

		// 타이머 정리
		GetWorld()->GetTimerManager().ClearTimer(BoostTimerHandle);

		// BoostHeal 애니메이션 중지
		StopBoostHealPulse();
	}	
}

void UPlayerStateWidget::StartBoostHealPulse()
{
	if (BoostHealPulse)
	{
		// 0.0초부터 애니메이션 시작, 무한 반복
		PlayAnimation(BoostHealPulse, 0.0f, 0);
	}
}

void UPlayerStateWidget::StopBoostHealPulse()
{
	if (BoostHealPulse)
	{
		StopAnimation(BoostHealPulse);
	}
}


void UPlayerStateWidget::StartBoostSpeedPulse()
{
	if (!IMG_BoostSpeed || !BoostSpeedPulse)
	{
		return;
	}

	// BoostSpeedPulse 애니메이션 재생
	PlayAnimation(BoostSpeedPulse, 0.0f, 0);
}

void UPlayerStateWidget::StopBoostSpeedPulse()
{
	if (!IMG_BoostSpeed || !BoostSpeedPulse)
	{
		return;
	}

	// BoostSpeedPulse 애니메이션 정지
	StopAnimation(BoostSpeedPulse);
}

// 체력 업데이트
void UPlayerStateWidget::UpdateHealth(const float NewCurrentHp, const float NewMaxHp)
{
	// 새로운 체력 값과 최대 체력 값을 설정
	CurrentHealth = FMath::Clamp(NewCurrentHp, 0.0f, NewMaxHp);
	MaxHealth=FMath::Max(NewMaxHp, 1.0f);

	// HPBar는 즉시 업데이트
	if (HPBar)
	{
		float HealthPercentage = CurrentHealth / MaxHealth;
		HPBar->SetPercent(HealthPercentage);
	
		// 체력에 따른 ProgressBar 색상 설정
		FLinearColor BarColor;

		if (HealthPercentage > 0.5f)
		{
			BarColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);

			// 체력이 25% 이상 회복되었으면 애니메이션 정지
			if (IsAnimationPlaying(HPBarPulse))
			{
				StopAnimation(HPBarPulse);
			}
		}
		else if (HealthPercentage > 0.25f)
		{
			BarColor = FLinearColor(1.0f, 0.745f, 0.259f, 1.0f);

			if (IsAnimationPlaying(HPBarPulse))
			{
				StopAnimation(HPBarPulse);
			}
		}
		else
		{
			BarColor = FLinearColor(0.5f, 0.0f, 0.0f, 1.0f);
			if (!IsAnimationPlaying(HPBarPulse))
			{
				PlayAnimation(HPBarPulse,0.0f, 0.0f);				
			}
		}
		HPBar->SetFillColorAndOpacity(BarColor);		
	}
	
	// DamageIndicatorBar 부드럽게 체력 감소 / 증가를 따라가도록 설정
	if (DamageIndicatorBar && !GetWorld()->GetTimerManager().IsTimerActive(DamageIndicatorTimerHandle))
	{
		// DamageIndicatorBar의 색상을 빨간색 또는 흰색으로 설정
		if (NewCurrentHp < CurrentHealth)
		{
			DamageIndicatorBar->SetFillColorAndOpacity(FLinearColor(1.0f, 0.0f, 0.0f, 0.7f));
		}
		else
		{
			DamageIndicatorBar->SetFillColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.7f));
		}		
		GetWorld()->GetTimerManager().SetTimer(
			DamageIndicatorTimerHandle,
			this,
			&UPlayerStateWidget::SmoothUpdateDamageIndicator,
			0.01f,
			true
			);
	}
}

// DamageIndicatorBar 업데이트
void UPlayerStateWidget::SmoothUpdateDamageIndicator()
{
	if (!DamageIndicatorBar)
	{
		return;
	}

	// 현재 데미지 인디케이터 값
	float CurrentPercent=DamageIndicatorBar->GetPercent();

	// DamageIndicatorBar의 목표 값
	float TargetPercent = CurrentHealth / MaxHealth;

	// 점점 0으로 감소
	CurrentPercent=FMath::FInterpTo(CurrentPercent, TargetPercent, GetWorld()->GetDeltaSeconds(), 5.0f);

	// 업데이트
	DamageIndicatorBar->SetPercent(CurrentPercent);

	// 목표 Percent에 거의 도달하면 타이머 종료
	if (FMath::IsNearlyEqual(CurrentPercent,TargetPercent,0.02f))
	{
		DamageIndicatorBar->SetPercent(TargetPercent);
		GetWorld()->GetTimerManager().ClearTimer(DamageIndicatorTimerHandle);
	}
}

void UPlayerStateWidget::UpdateAmmoOverlay(EGunType _GunType)
{
	if (WBP_AmmoSlot)
	{
		WBP_AmmoSlot->UpdateAmmoOverlay(_GunType);
	}
}

void UPlayerStateWidget::UpdateFireMode(const FGameplayTag& _FireMode)
{
	if (WBP_AmmoSlot)
	{
		WBP_AmmoSlot->UpdateFireMode(_FireMode);
	}
}


void UPlayerStateWidget::UpdateAmmoSlot(const bool _bTrigger,const EPlayerActionMode _PlayerActionMode, const EGunType _GunType, const FGameplayTag& _FireMode)
{
	if (CurrentAmmoText && PossessedAmmoText)
	{
		if (_bTrigger == false)
		{
			// TODO 알파값 1세팅
			CurrentAmmoText->SetVisibility(ESlateVisibility::HitTestInvisible);
			CurrentAmmoText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
				
			PossessedAmmoText->SetVisibility(ESlateVisibility::HitTestInvisible);
			PossessedAmmoText->SetColorAndOpacity(FSlateColor(FLinearColor::Gray));
			
			switch (_PlayerActionMode)
			{
			case EPlayerActionMode::Unarmed:
			case EPlayerActionMode::Melee:
				// 히든
				CurrentAmmoText->SetVisibility(ESlateVisibility::Collapsed);
				PossessedAmmoText->SetVisibility(ESlateVisibility::Collapsed);
				WBP_AmmoSlot->SetVisibility(ESlateVisibility::Collapsed);
				break;

			case EPlayerActionMode::Rifle:
				// 흰색
				CurrentAmmoText->SetVisibility(ESlateVisibility::HitTestInvisible);
				CurrentAmmoText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
				
				PossessedAmmoText->SetVisibility(ESlateVisibility::HitTestInvisible);
				PossessedAmmoText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.5f)));

				if (_GunType != EGunType::None)
				{
					WBP_AmmoSlot->UpdateAmmoOverlay(_GunType);
					WBP_AmmoSlot->UpdateFireMode(_FireMode);				
					WBP_AmmoSlot->SetVisibility(ESlateVisibility::HitTestInvisible);
				}				
				break;

			case EPlayerActionMode::Grenade:
				// 흰색
				CurrentAmmoText->SetVisibility(ESlateVisibility::HitTestInvisible);
				CurrentAmmoText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
				
				PossessedAmmoText->SetVisibility(ESlateVisibility::Collapsed);
				
				WBP_AmmoSlot->SetVisibility(ESlateVisibility::Collapsed);
				break;
			default:
				checkf(true,TEXT("EPlayerActionMode Error"));
					break;
			}
		}
		else
		{
			// 알파값 낮추기
			CurrentAmmoText->SetVisibility(ESlateVisibility::HitTestInvisible);
			CurrentAmmoText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.6f)));
				
			PossessedAmmoText->SetVisibility(ESlateVisibility::HitTestInvisible);
			PossessedAmmoText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.6f)));
			
			if (WBP_AmmoSlot->AR_Overlay->IsVisible())
			{
				WBP_AmmoSlot->IMG_Ammo_1->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
				WBP_AmmoSlot->IMG_Ammo_2->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
				WBP_AmmoSlot->IMG_Ammo_3->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
				WBP_AmmoSlot->IMG_Ammo_4->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
				WBP_AmmoSlot->IMG_Ammo_5->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
			}

			if (WBP_AmmoSlot->SR_Overlay->IsVisible())
			{
				WBP_AmmoSlot->IMG_SR_Ammo->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
			}

			if (WBP_AmmoSlot->SG_Overlay->IsVisible())
			{
				WBP_AmmoSlot->IMG_SG_Ammo->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
			}			
		}		
	}
}
