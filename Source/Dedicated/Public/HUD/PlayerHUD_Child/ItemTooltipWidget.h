// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"
#include "BaseType/BaseEnumType.h"
#include "ItemTooltipWidget.generated.h"

class UImage;
class UTextBlock;
/**
 *  InventoryUI에서 아이템 호버시 아이템 정보 출력
 */
UCLASS()
class DEDICATED_API UItemTooltipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	/**
	 *  ItemID를 검색하여 해당 정보를 토대로 이미지 설정
	 *
	 *  @param  _ItemID 아이템 넘버
	 */
	void SetupItemTooltip(const int32 _ItemID, const int32 _ItemCont = 0);


	/** 소제목에 표시되는 부분을 적용하는 함수*/
	FText GetItemType(const EGunType _ItemType);

	/** 소제목에 표시되는 부분을 적용하는 함수*/
	FText GetItemType(const EItemType _ItemType);

	/** 하단에 용량 표시 적용하는 함수
	 * 
	 * 무기면 스킵, 아이템이 1개면 단일 용량, 1개이상이면 총합 용량 표시
	 */
	FText GetItemWeight(const float _ItemWeight, const int32 _ItemCount, const EItemType _ItemType);


private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_ItemName;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_ItemType;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Item;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_ItemDesc;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_ItemWeight;
};
