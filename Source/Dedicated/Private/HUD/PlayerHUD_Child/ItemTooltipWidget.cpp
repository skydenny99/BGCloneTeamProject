// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/ItemTooltipWidget.h"

#include <ThirdParty/fmt/8.1.1/include/fmt/chrono.h>

#include "BaseDebugHelper.h"
#include "BaseType/BaseStructType.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"
#include "Subsystem/PubgDataTableManageSubsystem.h"


void UItemTooltipWidget::SetupItemTooltip(const int32 _ItemID, const int32 _ItemCont)
{
	if (const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(_ItemID))
	{
		Text_ItemName->SetText(ItemData->DisplayName);
		Text_ItemType->SetText(
			ItemData->ItemType == EItemType::Mainweapon || ItemData->ItemType == EItemType::Subweapon
				? GetItemType(UBaseFunctionLibrary::GetGunData(_ItemID)->GunType)
				: GetItemType(ItemData->ItemType)
		);

		Img_Item->SetBrushFromTexture(ItemData->ItemIcon);
		Text_ItemDesc->SetText(ItemData->Description);
		Text_ItemWeight->SetText(GetItemWeight(ItemData->ItemWeight, _ItemCont, ItemData->ItemType));
	}
}

FText UItemTooltipWidget::GetItemType(const EGunType _ItemType)
{
	switch (_ItemType)
	{
	case EGunType::AR:
		return FText::FromString(TEXT("돌격 소종 (AR)"));
	case EGunType::LMG:
		return FText::FromString(TEXT("경기관총 (LMG)"));
	case EGunType::DMR:
		return FText::FromString(TEXT("지정사수소총 (DMR)"));
	case EGunType::SR:
		return FText::FromString(TEXT("저격소총 (SR)"));
	case EGunType::SG:
		return FText::FromString(TEXT("산탄총 (SG)"));
	case EGunType::SMG:
		return FText::FromString(TEXT("기관단총 (SMG)"));
	case EGunType::Pistol:
		return FText::FromString(TEXT("권총"));
	default:
		return FText::FromString(TEXT("None"));
	}
}

FText UItemTooltipWidget::GetItemType(const EItemType _ItemType)
{
	switch (_ItemType)
	{
	case EItemType::Melee:
		return FText::FromString(TEXT("근접 무기"));
	case EItemType::Ammobox:
		return FText::FromString(TEXT("탄약"));
	case EItemType::Healing:
		return FText::FromString(TEXT("회복용"));
	case EItemType::Throwing:
		return FText::FromString(TEXT("투척 무기"));
	case EItemType::Vest:
		return FText::FromString(TEXT("방탄 조끼"));
	case EItemType::Backpack:
		return FText::FromString(TEXT("배낭"));
	case EItemType::Helmet:
		return FText::FromString(TEXT("머리 보호구"));
	case EItemType::Attachments:
		return FText::FromString(TEXT("총기 악세서리"));
	default:
		return FText::FromString(TEXT("None"));
	}
}


FText UItemTooltipWidget::GetItemWeight(const float _ItemWeight, const int32 _ItemCount, const EItemType _ItemType)
{
	if (_ItemType == EItemType::Mainweapon or _ItemType == EItemType::Subweapon)
	{
		return FText::GetEmpty();
	}
	else if (_ItemCount == -1 || _ItemWeight == 0)
	{
		return FText::FromString(FString::Printf(TEXT("%.0f 용량"),_ItemWeight));
	}
	else
	{
		return FText::FromString(FString::Printf(TEXT("%.0f(%.1fX%d) 용량"),_ItemWeight*_ItemCount, _ItemWeight, _ItemCount));
	}
}
