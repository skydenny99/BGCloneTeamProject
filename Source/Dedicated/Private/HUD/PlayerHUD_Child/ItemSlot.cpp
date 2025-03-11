// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/ItemSlot.h"

#include "BaseDebugHelper.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "HUD/PlayerHUD_Child/ItemSlotImage.h"
#include "Subsystem/PubgDataTableManageSubsystem.h"


void UItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (WBP_ItemImgae)
	{
		WBP_ItemImgae->SetVisibility(ESlateVisibility::Collapsed);
		Img_Disable->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (Btn_Slot)
	{
		Btn_Slot->OnHovered.AddDynamic(this, &ThisClass::OnSlotHovered);
		Btn_Slot->OnUnhovered.AddDynamic(this, &ThisClass::OnSlotNormal);
	}
}




void UItemSlot::SetSlotImage(UTexture2D* ItemIcon, const int32 _ItemID)
{
	if (_ItemID  <= 0)
	{
		if (ItemIcon)
		{
			WBP_ItemImgae->SetupImage(ItemIcon);
			WBP_ItemImgae->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		if (_ItemID != -1)
			ItemID = _ItemID;
		return;
	}
	
	const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(_ItemID);
	if (!ItemData) return;
	
	if (ItemIcon)
	{
		if (ItemData->ItemType == EItemType::Helmet || ItemData->ItemType == EItemType::Vest)
		{
			WBP_ItemImgae->SetupImage(ItemIcon,100);
		}
		else
		{
			WBP_ItemImgae->SetupImage(ItemIcon);
		}
		
			WBP_ItemImgae->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (_ItemID != -1)
		ItemID = _ItemID;
}

void UItemSlot::SetSlotImage(const int32 _ItemID)
{
	if (_ItemID <= 0)
	{
		Debug::Print("UItemSlot::SetSlotImage: ItemID is invalid");
		return;
	}

	const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(_ItemID);
	if (!ItemData)
	{
		Debug::Print("UItemSlot::SetSlotImage: Fail Load ItemData");
		return;
	}

	ItemID = _ItemID;
	WBP_ItemImgae->SetupImage(ItemData->ItemIcon);
	WBP_ItemImgae->SetVisibility(ESlateVisibility::HitTestInvisible);
	
}

void UItemSlot::InitSlotImage()
{
	if (WBP_ItemImgae->IsVisible())
		WBP_ItemImgae->SetVisibility(ESlateVisibility::Collapsed);
	ItemID = -1;
}

void UItemSlot::OnSlotNormal()
{
	Img_Normal->SetVisibility(ESlateVisibility::HitTestInvisible);
	Img_Disable->SetVisibility(ESlateVisibility::Collapsed);
	Img_Hovered->SetVisibility(ESlateVisibility::Collapsed);
	Img_Pressed->SetVisibility(ESlateVisibility::Collapsed);
}

void UItemSlot::OnSlotHovered()
{
	Img_Normal->SetVisibility(ESlateVisibility::Collapsed);
	Img_Disable->SetVisibility(ESlateVisibility::Collapsed);
	Img_Hovered->SetVisibility(ESlateVisibility::HitTestInvisible);
	Img_Pressed->SetVisibility(ESlateVisibility::Collapsed);
}

void UItemSlot::OnSlotPressed()
{
	Img_Normal->SetVisibility(ESlateVisibility::Collapsed);
	Img_Disable->SetVisibility(ESlateVisibility::Collapsed);
	Img_Hovered->SetVisibility(ESlateVisibility::Collapsed);
	Img_Pressed->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UItemSlot::OnSlotDisable()
{
	Img_Normal->SetVisibility(ESlateVisibility::Collapsed);
	Img_Disable->SetVisibility(ESlateVisibility::HitTestInvisible);
	Img_Hovered->SetVisibility(ESlateVisibility::Collapsed);
	Img_Pressed->SetVisibility(ESlateVisibility::Collapsed);
}

void UItemSlot::SetParts(const bool Bool)
{
	if (Bool)
	{
		Img_Normal->SetBrushFromTexture(Image_PartsIcons[0]);
		Img_Disable->SetBrushFromTexture(Image_PartsIcons[1]);
		Img_Hovered->SetBrushFromTexture(Image_PartsIcons[2]);
		Img_Pressed->SetBrushFromTexture(Image_PartsIcons[3]);
	}
	else
	{
		Img_Normal->SetBrushFromTexture(Image_ArmorIcons[0]);
		Img_Disable->SetBrushFromTexture(Image_ArmorIcons[1]);
		Img_Hovered->SetBrushFromTexture(Image_ArmorIcons[2]);
		Img_Pressed->SetBrushFromTexture(Image_ArmorIcons[3]);
	}
}
