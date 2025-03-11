// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/ItemSlotImage.h"

#include <chrono>

#include "Components/Image.h"
#include "Components/TextBlock.h"


void UItemSlotImage::SetupImage(UTexture2D* ItemImage)
{
	if (ItemImage)
		Img_Item->SetBrushFromTexture(ItemImage);
}

void UItemSlotImage::SetupImage(UTexture2D* ItemImage, const int32 Durability)
{
	if (ItemImage)
	{
		Img_Item->SetBrushFromTexture(ItemImage);
		Text_Durability->SetText(FText::AsNumber(Durability));
		Img_Durability->SetVisibility(ESlateVisibility::HitTestInvisible);
		Text_Durability->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UItemSlotImage::SetupDurability(const int32 Durability)
{
	Text_Durability->SetText(FText::AsNumber(Durability));
}
