// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/DragItemImage.h"

#include "BaseDebugHelper.h"
#include "Components/Image.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"
#include "Subsystem/PubgDataTableManageSubsystem.h"


void UDragItemImage::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void UDragItemImage::InitSlot(const int32 ItemID)
{
	if (ItemID <= 0) return;

	if(const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(ItemID))
	{
		Img_Item->SetBrushFromTexture(ItemData->ItemIcon);
		SetImageSelf();
	}
}

void UDragItemImage::SetImageSelf()
{
	Img_Self->SetVisibility(ESlateVisibility::HitTestInvisible);
	Img_Target->SetVisibility(ESlateVisibility::Collapsed);
	Img_Block->SetVisibility(ESlateVisibility::Collapsed);
	Img_Block_Ban->SetVisibility(ESlateVisibility::Collapsed);
	Img_Block_Frame->SetVisibility(ESlateVisibility::Collapsed);
}

void UDragItemImage::SetImageTarget()
{
	Img_Self->SetVisibility(ESlateVisibility::Collapsed);
	Img_Target->SetVisibility(ESlateVisibility::HitTestInvisible);
	Img_Block->SetVisibility(ESlateVisibility::Collapsed);
	Img_Block_Ban->SetVisibility(ESlateVisibility::Collapsed);
	Img_Block_Frame->SetVisibility(ESlateVisibility::Collapsed);
}

void UDragItemImage::SetImageBlock()
{
	Img_Self->SetVisibility(ESlateVisibility::Collapsed);
	Img_Target->SetVisibility(ESlateVisibility::Collapsed);
	Img_Block->SetVisibility(ESlateVisibility::HitTestInvisible);
	Img_Block_Ban->SetVisibility(ESlateVisibility::HitTestInvisible);
	Img_Block_Frame->SetVisibility(ESlateVisibility::HitTestInvisible);
}
