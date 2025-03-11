// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/PlayerStateSlot/AmmoSlotWidget.h"

#include "BaseDebugHelper.h"
#include "Components/Overlay.h"
#include "Components/Image.h"


void UAmmoSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (AR_Overlay)
	{
		AR_Overlay->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	
	if (SR_Overlay)
	{
		SR_Overlay->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (SG_Overlay)
	{
		SG_Overlay->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UAmmoSlotWidget::UpdateAmmoOverlay(EGunType _GunType)
{
	// 모든 오버레이 숨김
	if (AR_Overlay)
	{
		AR_Overlay->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (SR_Overlay)
	{
		SR_Overlay->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (SG_Overlay)
	{
		SG_Overlay->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 무기 타입에 따라 해당 오버레이 표시 및 이미지 처리
	switch (_GunType)
	{
	case EGunType::AR:
	case EGunType::SMG:
	case EGunType::Pistol:
	case EGunType::LMG:
		if (AR_Overlay)
		{
			AR_Overlay->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		break;

	case EGunType::SR:		
	case EGunType::DMR:
		if (SR_Overlay)
		{
			SR_Overlay->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		break;

	case EGunType::SG:
		if (SG_Overlay)
		{
			SG_Overlay->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		break;

	default:
		if (AR_Overlay)
		{
			AR_Overlay->SetVisibility(ESlateVisibility::Collapsed);
		}
	
		if (SR_Overlay)
		{
			SR_Overlay->SetVisibility(ESlateVisibility::Collapsed);
		}
	
		if (SG_Overlay)
		{
			SG_Overlay->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;		
	}
}

void UAmmoSlotWidget::UpdateFireMode(const FGameplayTag& _FireMode)
{
	// AR 오버레이가 활성 상태일 때만 작동
	// if (AR_Overlay && AR_Overlay->IsVisible())
	// {
		// 단발 모드
	if (_FireMode.MatchesTagExact(BaseGameplayTags::Weapon_Gun_FireMode_Single))
	{
		if (IMG_Ammo_1)
		{
			IMG_Ammo_1->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (IMG_Ammo_2)
		{
			IMG_Ammo_2->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));			
		}
		if (IMG_Ammo_3)
		{
			IMG_Ammo_3->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));			
		}
		if (IMG_Ammo_4)
		{
			IMG_Ammo_4->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));			
		}
		if (IMG_Ammo_5)
		{
			IMG_Ammo_5->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));			
		}
	}
	else if (_FireMode.MatchesTagExact(BaseGameplayTags::Weapon_Gun_FireMode_Burst))
	{
		if (IMG_Ammo_1)
		{
			IMG_Ammo_1->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (IMG_Ammo_2)
		{
			IMG_Ammo_2->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));			
		}
		if (IMG_Ammo_3)
		{
			IMG_Ammo_3->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));			
		}
		if (IMG_Ammo_4)
		{
			IMG_Ammo_4->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));			
		}
		if (IMG_Ammo_5)
		{
			IMG_Ammo_5->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));			
		}
	}
	else if (_FireMode.MatchesTagExact(BaseGameplayTags::Weapon_Gun_FireMode_Auto))
	{
		if (IMG_Ammo_1)
		{
			IMG_Ammo_1->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (IMG_Ammo_2)
		{
			IMG_Ammo_2->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (IMG_Ammo_3)
		{
			IMG_Ammo_3->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (IMG_Ammo_4)
		{
			IMG_Ammo_4->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (IMG_Ammo_5)
		{
			IMG_Ammo_5->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	else
	{
		Debug::PrintError("No Matched FireMode in AmmoSlotWidget.cpp");
	}
}

void UAmmoSlotWidget::DeactivateFireMode()
{
	if (IMG_Ammo_1)
	{
		IMG_Ammo_1->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
	}
	if (IMG_Ammo_2)
	{
		IMG_Ammo_2->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));			
	}
	if (IMG_Ammo_3)
	{
		IMG_Ammo_3->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));			
	}
	if (IMG_Ammo_4)
	{
		IMG_Ammo_4->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));			
	}
	if (IMG_Ammo_5)
	{
		IMG_Ammo_5->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));			
	}
}
