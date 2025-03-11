// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "GearWidget.generated.h"

class UDragItemImage;
class ABaseGearFieldItem;
class UItemSlot;
class UImage;
class UTextureRenderTarget2D;

/**
 *  InventoryUI에서 캐릭터와 방어구 슬롯 담당 Widget
 */
UCLASS()
class DEDICATED_API UGearWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UGearWidget(const FObjectInitializer& ObjectInitializer);

	/** 아무것도 안받으면 전체 스캔해서 적용 - 장착로직은 없음. */
	void SetGear();

	/** Inventory용 캐릭터 설정 */
	void SetRenderTarget(UTextureRenderTarget2D* RenderTarget);

	/**하이라이트 이벤트*/
	void ActiveHighLight(const FGameplayTag _GearTag);
	void DeActiveHighLight();

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	
private:

	/** Character UI Image */
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Character;

	/** RenderTarget을 담고있는 Material Instance */
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterialInstance;
	
	/** 핼멧 슬롯 */
	UPROPERTY(meta = (BindWidget))
	UItemSlot* Slot_Helmet;

	/** 가방 슬롯 */
	UPROPERTY(meta = (BindWidget))
	UItemSlot* Slot_Backpack;

	/** 조끼 슬롯 */
	UPROPERTY(meta = (BindWidget))
	UItemSlot* Slot_Vest;

	/** 길리슈트 전용 슬롯 */
	UPROPERTY(meta = (BindWidget))
	UItemSlot* Slot_Ghillie;

	/** 코스튬 핼멧 슬롯 */ 
	UPROPERTY(meta = (BindWidget))
	UItemSlot* CostumeSlot_Head;

	/** 코스튬 안경 슬롯 */
	UPROPERTY(meta = (BindWidget))
	UItemSlot* CostumeSlot_Glass;

	/** 코스튬 마스크 슬롯 */
	UPROPERTY(meta = (BindWidget))
	UItemSlot* CostumeSlot_Mask;

	/** 코스튬 상의 슬롯 */
	UPROPERTY(meta = (BindWidget))
	UItemSlot* CostumeSlot_Top;

	/** 코스튬 외투 슬롯 */
	UPROPERTY(meta = (BindWidget))
	UItemSlot* CostumeSlot_Outerwear;

	/** 코스튬 장갑 슬롯 */
	UPROPERTY(meta = (BindWidget))
	UItemSlot* CostumeSlot_Glove;

	/** 코스튬 하의 슬롯 */
	UPROPERTY(meta = (BindWidget))
	UItemSlot* CostumeSlot_Bottom;

	/** 코스튬 신발 슬롯 */
	UPROPERTY(meta = (BindWidget))
	UItemSlot* CostumeSlot_Boots;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf<UDragItemImage> DragItemWidgetClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	int32 HoveredSlotItemID;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	FGameplayTag HoveredSlotTag;
	
};
