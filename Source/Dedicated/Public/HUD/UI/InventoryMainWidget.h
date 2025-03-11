// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMainWidget.generated.h"


class UItemTooltipWidget;
class UGearWidget;
class UWeaponSlot;
class UVerticalBox;
class UScrollBarWidget;
class UScrollBox;
class UInventorySlotWidget;

/**
 * 
 */
UCLASS()
class DEDICATED_API UInventoryMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 인벤토리를 설정하는 함수 */
	UFUNCTION(BlueprintCallable)
	void UpdateInventory();

	UFUNCTION(BlueprintCallable)
	void UpdateAroundItem();

	UFUNCTION(BlueprintCallable)
	void UpdateWeapon();

	UFUNCTION(BlueprintCallable)
	void UpdateGear();

	void ActiveTooltipBox();
	void DeActiveTooltipBox();


	void UpdateWeaponParts();


	void ToggleFreeFallingWidget();
	
	FORCEINLINE UGearWidget* GetGearWidget() const {return WBP_Gear;}
	FORCEINLINE UItemTooltipWidget* GetTooltipWidget() const {return WBP_ItemTooltip;}

	void EnableHighLightWeaponSlot(const FGameplayTag _ItemTag);
	void DisableHighLightWeaponSlot(const FGameplayTag _ItemTag);
	void DisableHighLightWeaponSlot();
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	/** 인벤토리 슬롯을 표시할 ScrollBox */
	UPROPERTY(meta = (BindWidget))
	UScrollBarWidget* WBP_InventoryScrollBox;

	// 주변 아이템 ScrollBox
	UPROPERTY(meta = (BindWidget))
	UScrollBarWidget* WBP_AroundScrollBox;

	// ItemToolTip Box
	UPROPERTY(meta = (BindWidget))
	UItemTooltipWidget* WBP_ItemTooltip;
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_Primary;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_Secondary;

	UPROPERTY(meta = (BindWidget))
	UGearWidget* WBP_Gear;


	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess))
	bool bActiveTooltipBox;

	/** 슬롯 UI 클래스 (Blueprint에서 할당) */
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInventorySlotWidget> InventorySlotClass;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UWeaponSlot> WeaponSlotClass;
};
