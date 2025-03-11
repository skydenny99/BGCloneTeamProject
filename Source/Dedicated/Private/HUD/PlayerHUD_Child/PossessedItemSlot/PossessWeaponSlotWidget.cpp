// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/PossessedItemSlot/PossessWeaponSlotWidget.h"

#include "BaseDebugHelper.h"
#include "Components/Image.h"


void UPossessWeaponSlotWidget::SetupImage(UTexture2D* _Texture)
{
	Img_Weapon->SetBrushFromTexture(_Texture);
}

void UPossessWeaponSlotWidget::SetupNumber(const int32 _Number)
{
	if (UTexture2D** FoundTexture = NumberMap.Find(_Number))
	{
		Debug::Print("ChangeImage");
		Img_Number->SetBrushFromTexture(*FoundTexture);
	}
}

void UPossessWeaponSlotWidget::ToggleAlpha(const bool _Alpha)
{
	if (_Alpha)
	{
		if (Img_Weapon->GetRenderOpacity()!= 2.0f)
		{
			Debug::Print("Detected Alpha To Active : ",Img_Weapon->GetRenderOpacity());
			Img_Weapon->SetRenderOpacity(2.0f);
		}
	}
	else
	{
		if (Img_Weapon->GetRenderOpacity()!= 1.f)
		{
			Debug::Print("Detected Alpha To DeActive: ",Img_Weapon->GetRenderOpacity());
			Img_Weapon->SetRenderOpacity(1.f);
		}
	}

}
