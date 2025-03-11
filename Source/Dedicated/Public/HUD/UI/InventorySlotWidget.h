// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

struct FInventoryListItem;
struct FInventoryUI;
class ABaseFieldItem;
class UDragItemImage;
class UButton;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class DEDICATED_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UInventorySlotWidget(const FObjectInitializer& ObjectInitializer);
	/** 슬롯 데이터를 설정하는 함수 */ // == Legacy 이젠 잘 안쓸듯.
	void SetupSlot(UTexture2D* ImageIcon, const FString& ItemDisplayName, const int32 ItemAmount, ABaseFieldItem* _FieldItem = nullptr, const bool bIsArmor = false);

	// Inventory ScrollBox에 추가할 때 사용되는 함수. 인벤추가할떈 FInventoryUI 사용되도록 되어있음
	void SetupSlot(const FInventoryListItem& _ItemData);

	// Around ScrollBox에 추가할 때 사용되는 함수 추가할떈 BaseFieldItem을 기준으로 추가
	void SetupSlot(ABaseFieldItem* _FieldItem);

	FORCEINLINE void SetbIsInventory(const bool _Bool) { bIsInventory = _Bool; }
	FORCEINLINE bool GetbIsInventory() const { return bIsInventory; }

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 ItemID = -1;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 Item_Amount = -1;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ABaseFieldItem* FieldItem = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsInventory;
	
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_ItemSlot;

	UPROPERTY(meta = (BindWidget))
	UImage* Img_Normal_BG;

	UPROPERTY(meta = (BindWidget))
	UImage* Img_Hovered_BG;

	UPROPERTY(meta = (BindWidget))
	UImage* Img_Armor;

	UPROPERTY(meta = (BindWidget))
	class UItemSlotImage* WBP_ItemImage;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_ItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_ItemAmount;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf<UDragItemImage> DragItemWidgetClass;
	

public:
	void SetItemID(const int32 initItemID) {ItemID = initItemID;}
	
	UFUNCTION()
	void OnHoveredSlot();
	
	UFUNCTION()
	void OnUnHoveredSlot();

	/** 버튼 입력 감지하는 함수, 좌클릭 시 드래그, 우클릭 시 바로 장착 */
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	/** 버튼 좌클릭 후 드래그  시 DragWidget 생성 */
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
};
	
	
