// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseType/BaseEnumType.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"
#include "ItemSlotImage.generated.h"

class UTextBlock;
class UImage;
/**
 *  SlotWidget에서 Item Image를 담당하는 UI
 */
UCLASS()
class DEDICATED_API UItemSlotImage : public UUserWidget
{
	GENERATED_BODY()
	
private:
	/** 아이템 이미지 저장 */
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Item;

	/** 아이템의 내구도가 표시되는 별도 이미지 */
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Durability;

	/** 해당 방어구의 내구도 저장*/
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Durability;

	/** 해당 변수를 활용하여 아이템 종류 판별*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	EItemType ItemType = EItemType::Ammobox;

public:
	/** 내구도가 없는 아이템 이미지 설정 */
	void SetupImage(UTexture2D* ItemImage);

	/** 내구도가 있는 아이템 이미지 설정 */
	void SetupImage(UTexture2D* ItemImage, const int32 Durability);

	/** 내구도 설정 함수*/ 
	void SetupDurability(const int32 Durability);

	/** 타입 변경 함수*/
	void SetItemType(const EItemType _ItemType) { ItemType = _ItemType; }
};
