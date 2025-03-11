// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Input/Reply.h"
#include "ItemSlot.generated.h"

class UItemSlotImage;
class UDragItemImage;
class UImage;
class UButton;
/**
 *  무기의 파츠슬롯, 방어구 슬롯을 담당하는 UI
 */
UCLASS()
class DEDICATED_API UItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetSlotImage(UTexture2D* ItemIcon, const int32 _ItemID = -1);
	void SetSlotImage(const int32 _ItemID);
	void InitSlotImage();

	// 파츠용, 방어구용 UI를 별도 설정
	/** 해당 슬롯이 파츠용인지 Gear용인지 Setup하는 함수*/
	void SetParts(const bool Bool);

	UFUNCTION()
	void OnSlotNormal();

	UFUNCTION()
	void OnSlotHovered();

	UFUNCTION()
	void OnSlotPressed();

	UFUNCTION()
	void OnSlotDisable();
	
	FORCEINLINE FGameplayTag GetSlotTag() const { return SlotTag; }
	FORCEINLINE int32 GetItemID() const { return ItemID; }

	
private:
	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = true))
	int32 ItemID = -1;
	
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Slot;
	
	UPROPERTY(meta = (BindWidget))
	UItemSlotImage* WBP_ItemImgae;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Normal;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Disable;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Hovered;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Pressed;

	/** Gear용 변수이며, 기본형, 호버형, Disable, Enable에 맞는 이미지를 담고 있음 */
	UPROPERTY(meta = (BindWidget), Blueprintreadwrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<UTexture2D*> Image_ArmorIcons;

	/** Parts용 변수이며, 기본형, 호버형, Disable, Enable에 맞는 이미지를 담고 있음 */
	UPROPERTY(meta = (BindWidget), Blueprintreadwrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<UTexture2D*> Image_PartsIcons;

	/** Part 슬롯 또는 방어구 슬롯이 어떤 슬롯인지 판별할때 사용하는 변수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FGameplayTag SlotTag;
};
