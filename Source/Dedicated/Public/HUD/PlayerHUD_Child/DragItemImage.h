// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragItemImage.generated.h"

class UImage;
/**
 *  드래그 시 마우스에 부착 되는 이미지
 */
UCLASS()
class DEDICATED_API UDragItemImage : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	/** ItemID를 기준으로 검색하여 관련 정보 기입 */ 
	void InitSlot(const int32 ItemID);

	/** Widget 스타일을 기본형으로 지정*/
	void SetImageSelf();

	/** Widget 스타일을 장착가능한 슬롯에 위치하고 있다는 허용형으로 지정 */
	void SetImageTarget();

	/** Widget 스타일을 장착이 불가능한 슬롯에 위치하고 있다는 금지형으로 지정*/
	void SetImageBlock();
	
private:

	/** 아이콘 이미지 */
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Item;

	/** 기본형 이미지 */
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Self;

	/** 허용형 이미지 */
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Target;

	/** 금지형 이미지 */
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Block;

	/** 좌측에 새로 추가되는 프레임 */
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Block_Frame;

	/** 죄측에 추가된 작은 상자에 설정된 이미지*/
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Block_Ban;
	
};
