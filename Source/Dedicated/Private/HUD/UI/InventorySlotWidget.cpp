// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/UI/InventorySlotWidget.h"

#include "BaseDebugHelper.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "HUD/PlayerHUD_Child/ItemSlotImage.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Characters/PubgCharacter.h"
#include "Components/Renewal/InventoryManageComponent.h"
#include "Components/Renewal/PickUpManageComponent.h"
#include "HUD/HUDBase.h"
#include "HUD/PlayerHUD_Child/DragItemImage.h"
#include "HUD/PlayerHUD_Child/ItemDragDropOperation.h"
#include "HUD/PlayerHUD_Child/ItemTooltipWidget.h"
#include "HUD/UI/InventoryMainWidget.h"
#include "Item/BaseFieldItem.h"
#include "Item/BaseGearFieldItem.h"
#include "Subsystem/PubgDataTableManageSubsystem.h"

UInventorySlotWidget::UInventorySlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UDragItemImage> DragWidgetAsset(TEXT("/Game/_BP/UI/Inventory/Module/WBP_DragItemImgae.WBP_DragItemImgae_C"));
	if (DragWidgetAsset.Succeeded())
		DragItemWidgetClass = DragWidgetAsset.Class;
}

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
	
	if (Btn_ItemSlot)
	{
		Btn_ItemSlot->OnHovered.AddDynamic(this, &ThisClass::OnHoveredSlot);
		Btn_ItemSlot->OnUnhovered.AddDynamic(this, &ThisClass::OnUnHoveredSlot);
	}
}

void UInventorySlotWidget::OnHoveredSlot()
{
	Img_Hovered_BG->SetVisibility(ESlateVisibility::HitTestInvisible);
	Img_Normal_BG->SetVisibility(ESlateVisibility::Collapsed);

	if (APubgCharacter* PlayerController = Cast<APubgCharacter>(GetOwningPlayerPawn()))
	{
		PlayerController->GetHUDBase()->GetInventoryMainWidget()->GetTooltipWidget()->SetupItemTooltip(ItemID,Item_Amount);
		PlayerController->GetHUDBase()->GetInventoryMainWidget()->ActiveTooltipBox();
	}
}

void UInventorySlotWidget::OnUnHoveredSlot()
{
	Img_Hovered_BG->SetVisibility(ESlateVisibility::Collapsed);
	Img_Normal_BG->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (APubgCharacter* PlayerController = Cast<APubgCharacter>(GetOwningPlayerPawn()))
	{
		PlayerController->GetHUDBase()->GetInventoryMainWidget()->DeActiveTooltipBox();
	}
}



void UInventorySlotWidget::SetupSlot(UTexture2D* ImageIcon, const FString& ItemDisplayName, const int32 ItemAmount, ABaseFieldItem* _FieldItem, const bool bIsArmor)
{
	// Non Armor
	if (!bIsArmor)
	{
		WBP_ItemImage->SetupImage(ImageIcon);
		Text_ItemName->SetText(FText::FromString(ItemDisplayName));
		Text_ItemAmount->SetText(FText::AsNumber(ItemAmount));
	}
	else // Armor
	{
		WBP_ItemImage->SetupImage(ImageIcon, ItemAmount);
		Text_ItemName->SetText(FText::FromString(ItemDisplayName));
	}

	Item_Amount = ItemAmount;
	if (_FieldItem)
		FieldItem = _FieldItem;
}

void UInventorySlotWidget::SetupSlot(const FInventoryListItem& _ItemData)
{
	ItemID = _ItemData.ItemID;
	Item_Amount = _ItemData.ItemAmount;
	EItemType ItemType = EItemType::Ammobox;

	if(const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(ItemID))
	{
		ItemType = ItemData->ItemType;
		if (ItemType == EItemType::Helmet || ItemType == EItemType::Vest)
		{
			WBP_ItemImage->SetupImage(ItemData->ItemIcon, Item_Amount);
			Text_ItemName->SetText(ItemData->DisplayName);
		}
		else
		{
			WBP_ItemImage->SetupImage(ItemData->ItemIcon);
			Text_ItemName->SetText(ItemData->DisplayName);
			Text_ItemAmount->SetText(FText::AsNumber(Item_Amount));
		}
	}
}

void UInventorySlotWidget::SetupSlot(ABaseFieldItem* _FieldItem)
{
	if (!_FieldItem) return;
	FieldItem = _FieldItem;
	ItemID = FieldItem->GetItemID();

	if(const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(ItemID))
	{
		Item_Amount = FieldItem->GetItemAmount();
		EItemType ItemType = ItemData->ItemType;
		UTexture2D* ItemIcon = ItemData->ItemIcon;
		FText ItemDisplayName =	ItemData->DisplayName;
		
		if (ItemType == EItemType::Ammobox) // 총알일 경우 갯수, 장비 표시 활성화 금지.
		{
			WBP_ItemImage->SetupImage(ItemIcon);
			Text_ItemName->SetText(ItemDisplayName);
			Text_ItemAmount->SetText(FText::AsNumber(Item_Amount));
		}
		else if (ItemType == EItemType::Helmet || ItemType == EItemType::Vest) // 헬멧과 조끼의 경우 내구도 표시 활성화
		{
			if (ABaseGearFieldItem* GearFieldItem = Cast<ABaseGearFieldItem>(FieldItem))
			{
				WBP_ItemImage->SetupImage(ItemIcon, GearFieldItem->GetCurrentDurability());
				Text_ItemName->SetText(ItemDisplayName);
				Text_ItemAmount->SetVisibility(ESlateVisibility::Collapsed);
				Img_Armor->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
		else // 나머지 ( 무기, 파츠 등 ) 은 수량 비활성화 
		{
			WBP_ItemImage->SetupImage(ItemIcon);
			Text_ItemName->SetText(ItemDisplayName);
			Text_ItemAmount->SetVisibility(ESlateVisibility::Collapsed);
			Img_Armor->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}

FReply UInventorySlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		Debug::Print("Left Clicked!");
		return  UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent,this,EKeys::LeftMouseButton).NativeReply;
	}
	
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		Debug::Print("Right Clicked!");
		if (APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(GetOwningPlayerPawn()))
		{
			PlayerCharacter->GetPickUpManageComponent()->PickUp(FieldItem);
		}
	}
	
	return FReply::Unhandled();
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                                UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UDragItemImage* DragWidget = nullptr;
	if (DragItemWidgetClass) // 드래그 시 마우스 포인터에 부착된 UI 설정, 
	{
		DragWidget = CreateWidget<UDragItemImage>(GetWorld(),DragItemWidgetClass);
		DragWidget->InitSlot(ItemID);
	}

	UItemDragDropOperation* DragOperation = Cast<UItemDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UItemDragDropOperation::StaticClass()));

	// 특정 공간에 투척할 경우 전달할 객체 설정
	if (DragOperation && IsValid(DragWidget))
	{
		DragOperation->DefaultDragVisual = DragWidget;
		DragOperation->Initialize(ItemID, Item_Amount, FieldItem,bIsInventory);
	}

	//하이라이트 효과 : 해당 아이템이 무기류 인지 체크. 
	if (APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(GetOwningPlayerPawn()))
	{
		if (FieldItem)
			PlayerCharacter->GetHUDBase()->GetInventoryMainWidget()->EnableHighLightWeaponSlot(FieldItem->GetItemType());
		else if (UBaseFunctionLibrary::GetItemData(ItemID)->ItemType == EItemType::Attachments)
			PlayerCharacter->GetHUDBase()->GetInventoryMainWidget()->EnableHighLightWeaponSlot(BaseGameplayTags::Item_Parts);
	}
	OutOperation = DragOperation;
}
