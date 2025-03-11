// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStateSlot/BagSlotWidget.h"
#include "PlayerStateSlot/VestSlotWidget.h"
#include "PlayerStateSlot/HelmetSlotWidget.h"
#include "PlayerStateSlot/AmmoSlotWidget.h"
#include "Components/TextBlock.h"
#include "BaseGameplayTags.h"
#include "BaseType/BaseEnumType.h"
#include "PlayerStateWidget.generated.h"

class UProgressBar;
class UImage;
class UTextBlock;
class UWidgetAnimation;

/**
 * 플레이어 상태 위젯
 */

UCLASS()
class DEDICATED_API UPlayerStateWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UBagSlotWidget* WBP_BagSlot;

	UPROPERTY(meta = (BindWidget))
	UHelmetSlotWidget* WBP_HelmetSlot;

	UPROPERTY(meta = (BindWidget))
	UVestSlotWidget* WBP_VestSlot;

	UPROPERTY(meta = (BindWidget))
	UAmmoSlotWidget* WBP_AmmoSlot;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentAmmoText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PossessedAmmoText;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_BoostSpeed;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_BoostHeal;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_Breath;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HPBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* DamageIndicatorBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* BoostBar;

	// 위젯 애니메이션
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* BoostHealPulse;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* BoostSpeedPulse;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* AmmoDecreaseAnim;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* HPBarPulse;

private:
	// 부드러운 체력 업데이트를 위한 변수
	float CurrentHealth=100.0f; // 현재 체력(디버깅용 초기값)
	float MaxHealth=100.0f;     // 최대 체력(디버깅용 초기값)

	// Boost 관련 변수
	float CurrentBoost = 0.0f;	// BoostBar의 현재 값
	float MaxBoost = 100.0f;	// BoostBar의 최대 값

	// BoostBar 감소 타이머
	FTimerHandle BoostTimerHandle;

	// DamageIndicatorBar 타이머 핸들
	FTimerHandle DamageIndicatorTimerHandle;

public:
	// 체력 업데이트 함수
	void UpdateHealth(const float NewCurrentHp,const float NewMaxHp);
	
	// BoostBar 증가
	void SetBoost(float NewCurrentBoost, float NewMaxBoost);

	// BoostHeal 애니메이션 시작 및 중지
	void StartBoostHealPulse();
	void StopBoostHealPulse();

	// BoostSpeed 애니메이션 시작 및 중지
	void StartBoostSpeedPulse();
	void StopBoostSpeedPulse();

	// DamageIndicatorBar를 부드럽게 업데이트하는 함수
	void SmoothUpdateDamageIndicator();

	// 총기 타입에 따라 오버레이 업데이트
	void UpdateAmmoOverlay(EGunType _GunType);

	// 발사 모드에 따라 탄약 상태 업데이트
	void UpdateFireMode(const FGameplayTag& _FireMode);

	// 총 소지 상태에 따른 AmmoSlot 업데이트
	void UpdateAmmoSlot(const bool _bTrigger = 0, const EPlayerActionMode _PlayerActionMode = EPlayerActionMode::Prone, const EGunType _GunType = EGunType::None, const FGameplayTag& _FireMode = BaseGameplayTags::Weapon_Gun_FireMode_Single);
	

public:
	virtual void NativeConstruct() override;

protected:
	// 타이머 함수
	void DecreaseBoostOverTime();

public:
	FORCEINLINE UBagSlotWidget* GetBagSlotWidget(){return WBP_BagSlot;}
	FORCEINLINE UHelmetSlotWidget* GetHelmetSlotWidget(){return WBP_HelmetSlot;}
	FORCEINLINE UVestSlotWidget* GetVestSlotWidget(){return WBP_VestSlot;}
	FORCEINLINE UAmmoSlotWidget* GetAmmoSlotWidget(){return WBP_AmmoSlot;}
	

	// 가방 게이지 업데이트
	FORCEINLINE void UpdateBagGuage(float _Min, float _Max, float _CurrentBagWeight)
	{
		if (WBP_BagSlot)
		{
			WBP_BagSlot->UpdateBagGuage(_Min, _Max, _CurrentBagWeight);
		}
	}

	// 가방 레벨에 따른 Texture 업데이트
	FORCEINLINE void UpdateBagBarTexture(int32 _BagLevel)
	{
		if (WBP_BagSlot)
		{
			WBP_BagSlot->UpdateBagBarTexture(_BagLevel);
		}
	}

	// 조끼 게이지 업데이트
	FORCEINLINE void UpdateVestDurability(float _NewDurability)
	{
		if (WBP_VestSlot)
		{
			WBP_VestSlot->UpdateVestDurability(_NewDurability);
		}
	}

	// 조끼 텍스쳐 레벨 업데이트
	FORCEINLINE void UpdateVestBarTexture(int32 _NewVestLevel)
	{
		if (WBP_VestSlot)
		{
			WBP_VestSlot->UpdateVestBarTexture(_NewVestLevel);
		}
	}

	// 헬멧 게이지 업데이트
	FORCEINLINE void UpdateHelmetDurability(float _NewDurability)
	{
		if (WBP_HelmetSlot)
		{
			WBP_HelmetSlot->UpdateHelmetDurability(_NewDurability);
		}
	}

	// 헬멧 텍스쳐 레벨 업데이트
	FORCEINLINE void UpdateHelmetBarTexture(int32 _NewVestLevel)
	{
		if (WBP_HelmetSlot)
		{
			WBP_HelmetSlot->UpdateHelmetBarTexture(_NewVestLevel);
		}
	}

	// 발사 후 남은 총알 수
	FORCEINLINE void UpdateCurrentDecreaseAmmo(int32 _CurrentAmmoAfterFire)
	{
		if (CurrentAmmoText)
		{
			// 잔여 탄약 표시
			CurrentAmmoText->SetText(FText::AsNumber(_CurrentAmmoAfterFire));

			// 0발일 경우 빨간색, 그렇지 않으면 흰색으로 설정
			FSlateColor TextColor;
			if (_CurrentAmmoAfterFire==0)
			{
				TextColor=FSlateColor(FLinearColor::Red);
			}
			else
			{
				TextColor=FSlateColor(FLinearColor::White);
			}

			// 텍스트 색상 변경
			CurrentAmmoText->SetColorAndOpacity(TextColor);

			// 애니메이션 재생
			if (AmmoDecreaseAnim)
			{
				PlayAnimation(AmmoDecreaseAnim, 0.0f, 1.0f);
			}
		}		
	}

	// HUD 현재 총알 업데이트
	FORCEINLINE void SetCurrentAmmo(int32 Val)
	{
		if (CurrentAmmoText)
		{
			CurrentAmmoText->SetText(FText::AsNumber(Val));
		}
	}

	
	
	// 인벤토리에 소유한 총알 수
	FORCEINLINE void UpdatePossessedAmmo(int32 _PossessedAmmo)
	{
		if (PossessedAmmoText)
		{
			// 잔여 탄약 표시
			PossessedAmmoText->SetText(FText::AsNumber(_PossessedAmmo));
			FLinearColor GrayTextColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.3f);
			PossessedAmmoText->SetColorAndOpacity(GrayTextColor);
		}		
	}

	// 장비 장착해제시 HUD 변경사항
	FORCEINLINE void UnEquipWeapon()
	{
		FLinearColor GrayTextColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.3f);
		CurrentAmmoText->SetColorAndOpacity(GrayTextColor);
		WBP_AmmoSlot->DeactivateFireMode();
	}
	
	


	

	// 가방에 아이템 추가 / 제거
	/*FORCEINLINE void AddItemToBag(float ItemWeight)
	{
		if (WBP_BagSlot)
		{
			WBP_BagSlot->AddItem(ItemWeight);
		}
	}*/
	
	/*FORCEINLINE void RemoveItemFromBag(float ItemWeight)
	{
		if (WBP_BagSlot)
		{
			WBP_BagSlot->RemoveItem(ItemWeight);
		}
	}*/

	/*// 조끼 착용 여부 설정
	FORCEINLINE void SetVestEquipped(bool bEquipped)
	{
		if (WBP_VestSlot)
		{
			WBP_VestSlot->SetVestEquipped(bEquipped);
		}
	}*/

	/*// 총 최대 무게 계산(가방 + 조끼)
	FORCEINLINE float GetTotalMaxWeight() const
	{
		float BagWeight=WBP_BagSlot ? WBP_BagSlot->GetMaxBagWeight() : 50.0f;
		float VestWeight=WBP_VestSlot ? WBP_VestSlot->GetVestWeightBonus() : 0.0f;
		return BagWeight + VestWeight;
	}*/

	// 현재 가방 무게 반환
	/*FORCEINLINE float GetCurrentBagWeight() const
	{
		return WBP_BagSlot ? WBP_BagSlot->GetCurrentBagWeight() : 0.0f;
	}	*/
};
