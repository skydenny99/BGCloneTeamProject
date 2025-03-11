// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD/PlayerHUD_Child/CompassWidget.h"
#include "HUD/PlayerHUD_Child/MiniMapWidget.h"
#include "HUD/PlayerHUD_Child/PlayerStateWidget.h"
#include "HUD/PlayerHUD_Child/PossessedItemWidget.h"
#include "HUD/PlayerHUD_Child/VehicleStateWidget.h"
#include "HUD/PlayerHUD_Child/KillFeedWidget.h"

#include "BaseType/BaseEnumType.h"
#include "BaseGameplayTags.h"
#include "HUD/PlayerHUD_Child/FreeFallingWidget.h"

#include "PlayerHUD.generated.h"

class UFreeFallingWidget;
class UHorizontalBox;
class UVerticalBox;
class UTextBlock;
class UWidgetAnimation;
class UInteract;

DECLARE_DELEGATE_OneParam(FFireCurrentAmmoDelegate, int32);
DECLARE_DELEGATE_OneParam(FKillCountDelegate, int32);
DECLARE_DELEGATE_ThreeParams(FKillFeedDelegate, const FString&, const FString&, int32);

/**
 * 플레이어 HUD
 */
UCLASS()
class DEDICATED_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//UPlayerHUD(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 현재 총에 장착된 총알 감소 델리게이트
	FFireCurrentAmmoDelegate OnFireCurrentAmmo;

	// 킬 피드 델리게이트
	FKillFeedDelegate OnKillFeed;

	// 킬 카운터 델리게이트
	FKillCountDelegate OnKillCount;

public:
	// 총기 타입과 아이콘 매핑
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponIcon", meta=(AllowPrivateAccess=true))
	TMap<int32, UTexture2D*> WeaponIconMap;

public:
	UPROPERTY(meta = (BindWidget))
	UCompassWidget* WBP_Compass;
	
	UPROPERTY(meta = (BindWidget))
	UMiniMapWidget* WBP_MiniMap;

	UPROPERTY(meta = (BindWidget))
	UPossessedItemWidget* WBP_PossessedItem;
	
	UPROPERTY(meta = (BindWidget))
	UPlayerStateWidget* WBP_PlayerState;

	UPROPERTY(meta = (BindWidget))
	UVehicleStateWidget* WBP_VehicleState;

	/*UPROPERTY(meta = (BindWidget))
	UKillFeedWidget* WBP_KillFeed;*/

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountSurvivorsText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillCount;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VER_KillFeedBox;
	
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HOR_KillCountBox;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* KillCountVisible;

	UPROPERTY(meta = (BindWidget))
	UInteract* WBP_Interact;

	UPROPERTY(EditAnywhere, Category = "KillFeed")
	TSubclassOf<UKillFeedWidget> KillFeedSlotClass;


#pragma region FreeFalling
public:
	void TriggerFreeFallingWidget(const bool _Active);
	
private:
	UPROPERTY(meta = (BindWidget))
	UFreeFallingWidget* WBP_FreeFalling;

	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess))
	bool bActiveFreeFallingWidget = false;
#pragma endregion

public:
	// 체력 업데이트 함수, 멤버 함수 선언
	// PlayerStateWidget 함수
	void UpdateHealth(const float NewCurrentHp,const float NewMaxHp);

	// 총기 타입에 따라 오버레이 업데이트
	void UpdateAmmoOverlay(EGunType _GunType);

	// 발사 모드에 따라 탄약 상태 업데이트
	UFUNCTION()
	void UpdateFireMode(const FGameplayTag& _FireMode);

	// 발사 후 남은 총알 수
	void UpdateCurrentDecreaseAmmo(int32 _CurrentAmmoAfterFire);

	// 단순 총알 수 업데이트
	UFUNCTION(BlueprintCallable)
	void SetHUDCurrentAmmo(int32 Val);
	
	// 인벤토리에 소유한 총알 수
	void UpdatePossessedAmmo(int32 _PossessedAmmo);

	// 총 소지 상태에 따른 AmmoSlot 업데이트
	void UpdateAmmoSlot(const bool _bTrigger = 0, const EPlayerActionMode _PlayerActionMode = EPlayerActionMode::Prone, const EGunType _GunType = EGunType::None, const FGameplayTag& _FireMode = BaseGameplayTags::Weapon_Gun_FireMode_Single);

	// 킬 카운트
	void UpdateKillCount(int32 _KillCount);

	// 킬 피드 로그
	void UpdateKillFeed(const FString& _Player, const FString& _Enemy, int32 _GunID = 1);
	void InitializeWeaponIcon();


public:
	// On/Off
	UFUNCTION(BlueprintCallable)
	void ToggleInteractWidget(const bool Active);

	void SetInteractWidgetSize();

	void UpdatePossessWeapon();

	
public:
	// Getter 함수
	FORCEINLINE UCompassWidget* GetCompassWidget(){return WBP_Compass;}
	FORCEINLINE UMiniMapWidget* GetMiniMapWidget(){return WBP_MiniMap;}
	FORCEINLINE UPossessedItemWidget* GetPossessedItemWidget(){return WBP_PossessedItem;}
	FORCEINLINE UPlayerStateWidget* GetPlayerStatusWidget(){return WBP_PlayerState;}
	FORCEINLINE UVehicleStateWidget* GetVehicleStateWidget(){return WBP_VehicleState;}
	FORCEINLINE UInteract* GetInteractWidget(){return WBP_Interact;}
	FORCEINLINE UFreeFallingWidget* GetFreeFallingWidget(){return WBP_FreeFalling;}
	FORCEINLINE float GetDistanceToGround() const { return WBP_FreeFalling->GetDistance();}

	// 가방 게이지 업데이트
	FORCEINLINE void UpdateBagGuage(float _Min, float _Max, float _CurrentWeight)
	{
		if (WBP_PlayerState)
		{
			WBP_PlayerState->UpdateBagGuage(_Min, _Max, _CurrentWeight);
		}
	}

	// 가방 레벨에 따른 Texture 업데이트
	FORCEINLINE void UpdateBagBarTexture(int32 _BagLevel)
	{
		if (WBP_PlayerState)
		{
			WBP_PlayerState->UpdateBagBarTexture(_BagLevel);
		}
	}

	// 조끼 게이지 업데이트
	FORCEINLINE void UpdateVestDurability(float _NewDurability)
	{
		if (WBP_PlayerState)
		{
			WBP_PlayerState->UpdateVestDurability(_NewDurability);
		}
	}

	// 조끼 텍스쳐 레벨 업데이트
	FORCEINLINE void UpdateVestBarTexture(int32 _NewVestLevel)
	{
		if (WBP_PlayerState)
		{
			WBP_PlayerState->UpdateVestBarTexture(_NewVestLevel);
		}
	}

	// 생존자 수 카운트
	FORCEINLINE void UpdateCountSurvivors(int32 _SurvivorsCount)
	{
		if (CountSurvivorsText)
		{
			CountSurvivorsText->SetText(FText::AsNumber(_SurvivorsCount));
		}
	}

	// 장비 장착해제시 HUD 변경사항
	FORCEINLINE void UnEquipWeapon()
	{
		if (WBP_PlayerState)
		{
			WBP_PlayerState->UnEquipWeapon();
		}
	}

	// 아이템 추가 / 제거
	/*FORCEINLINE void AddItemToBag(float ItemWeight)
	{
		if (WBP_PlayerState)
		{
			WBP_PlayerState->AddItemToBag(ItemWeight);
		}
	}

	FORCEINLINE void RemoveItemFromBag(float ItemWeight)
	{
		if (WBP_PlayerState)
		{
			WBP_PlayerState->RemoveItemFromBag(ItemWeight);
		}
	}*/

	/*// 조끼 착용 여부 설정
	FORCEINLINE void SetVestEquipped(bool bEquipped)
	{
		if (WBP_PlayerState)
		{
			WBP_PlayerState->SetVestEquipped(bEquipped);
		}
	}

	// 총 최대 무게 반환
	FORCEINLINE float GetTotalMaxWeight() const
	{
		return WBP_PlayerState ? WBP_PlayerState->GetTotalMaxWeight() : 50.0f;
	}*/

	// 현재 가방 무게 반환
	/*FORCEINLINE float GetCurrentBagWeight() const
	{
		return WBP_PlayerState ? WBP_PlayerState->GetCurrentBagWeight() : 0.0f;
	}	*/
};