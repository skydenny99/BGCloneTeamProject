// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/ScrollBarWidget.h"

#include "BaseDebugHelper.h"
#include "Blueprint/DragDropOperation.h"
#include "Characters/PubgCharacter.h"
#include "Components/BasePickUpComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Renewal/PickUpManageComponent.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "HUD/HUDBase.h"
#include "HUD/PlayerHUD_Child/ItemDragDropOperation.h"
#include "HUD/UI/InventoryMainWidget.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"
#include "Subsystem/PubgDataTableManageSubsystem.h"


void UScrollBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Setup Around
	if (bIsAround)
	{
		Text_Title->SetText(FText::FromString(TEXT("주변")));
		Text_Category_One->SetText(FText::FromString(TEXT("바닥")));
		Text_Category_Two->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Text_Title->SetText(FText::FromString(TEXT("인벤토리")));
		Text_Category_One->SetText(FText::FromString(TEXT("유형")));
		Text_Category_Two->SetText(FText::FromString(TEXT("최근 순")));
	}
}

void UScrollBarWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	if (UItemDragDropOperation* DragDropOperation = Cast<UItemDragDropOperation>(InOperation))
	{		
		if (DragDropOperation->GetIsInventory() && bIsAround)
			Img_Scroll_BG->SetVisibility(ESlateVisibility::HitTestInvisible);
		else
		{
			if(const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(DragDropOperation->GetItemID()))
			{
				if (ItemData->ItembIsStorable && !bIsAround && !DragDropOperation->GetIsInventory())
					Img_Scroll_BG->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
	}
}

void UScrollBarWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	if (Img_Scroll_BG->GetVisibility() == ESlateVisibility::HitTestInvisible)
		Img_Scroll_BG->SetVisibility(ESlateVisibility::Collapsed);
}

bool UScrollBarWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (UItemDragDropOperation* DragDropOperation = Cast<UItemDragDropOperation>(InOperation))
	{
		Img_Scroll_BG->SetVisibility(ESlateVisibility::Collapsed);
		APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(GetOwningPlayerPawn());
		if (!PlayerCharacter)
		{
			Debug::Print("Cast Failed PubgCharacter");
			return false;
		}

		// 이게 True면 바깥으로 드래그 했다는 뜻
		if (bIsAround)
		{
			// 버리기
			if (!DragDropOperation->GetWeaponTag().IsValid()) // 아이템 버리기
			{
				PlayerCharacter->GetPickUpManageComponent()->Server_DiscardItem( DragDropOperation->GetItemID(), DragDropOperation->GetItemAmount());
			}
			else // 무기 버리기
			{
				Debug::Print(FString::Printf(TEXT("Drop Weapon,  Tag : %s"),*DragDropOperation->GetWeaponTag().ToString()));
				if (DragDropOperation->GetPartsSlot().IsValid())
				{
					PlayerCharacter->GetPickUpManageComponent()->Server_DiscardWeaponParts(DragDropOperation->GetWeaponTag(), DragDropOperation->GetPartsSlot());
					Debug::Print("Discard Parts ::" +DragDropOperation->GetWeaponTag().ToString() );
					Debug::Print("Discard Parts ::" +DragDropOperation->GetPartsSlot().ToString() );
					 return true;
				}

				
				if (DragDropOperation->GetWeaponTag().MatchesTag(BaseGameplayTags::Item_Slot))
				{
					PlayerCharacter->GetPickUpManageComponent()->Server_DiscardWeapon(DragDropOperation->GetWeaponTag());	
				}
				else if (DragDropOperation->GetWeaponTag().MatchesTag(BaseGameplayTags::Item_Gear))
				{
					PlayerCharacter->GetPickUpManageComponent()->Server_DiscardGear(DragDropOperation->GetWeaponTag());
				}

				
			}
		}
		else // 인벤토리 슬롯에 드래그 시
		{
			if(const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(DragDropOperation->GetItemID()))
			{
				if (ItemData->ItembIsStorable)
					PlayerCharacter->GetPickUpManageComponent()->PickUp(DragDropOperation->GetFieldItem());
			}
		}
	}
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
