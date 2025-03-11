// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "BaseType/BaseEnumType.h"
#include "WeaponSlot.generated.h"

class ABaseWeaponFieldItem;
class UItemSlot;
class UButton;
class UTextBlock;
class UImage;
class UTexture2D;
class UDragItemImage;
/**
 * 
 */
UCLASS()
class DEDICATED_API UWeaponSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UWeaponSlot(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;

	// 메인이 되는 함수, 전달 받은 정보를 토대로 세팅 
	void SetupWeaponSlot(const int32 ItemItemID,const int32 CurrentAmmo, const int32 TotalAmmo, const FGameplayTag& WeaponType);
	
	// 무기 이미지 세팅
	void SetWeaponImage();

	// 파츠 슬롯 활성화
	void EnablePartsSlot(const bool bMuzzle, const bool bMagazine, const bool bStock, const bool bGrip, const bool bScope, const bool bSideScope);
	
	// ItemID를 토대로 각 맞는 파츠슬롯 활성화
	void InitWeaponSlot();

	// AmmoType에 맞는 정보 현행화
	void SetupAmmoType(EBulletType AmmoType);
	
	// Tag를 기준으로 슬롯 번호 설정
	void SetupSlotNumber(const FGameplayTag& WeaponType);
	
	// 아무것도 없을 시 비활성화
	void DisableWidget();
	
	// 활성화
	void EnableWidget();
	
	// Drag 활성화
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	// 클릭 한 상태로 움직이면 호출되는 함수
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	// 무엇이든 드래스 상태로 해당 위젯에 들어왔을 시 호출되는 함수 = BeginOverlap
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	// 무엇이든 드래스 상태로 해당 위젯에 들어왔을 시 호출되는 함수 = EndOverlap
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// 드래그 후 해당 위젯에 투척하면 호출 되는 함수
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	//무기 텍스처 가져오는 함수
	UTexture2D* GetWeaponTexture(const int32 _ItemID);

	UFUNCTION()
	void OnSlotHovered();

	UFUNCTION()
	void OnSlotUnHovered();

	void SlotPicked();

	void SlotPressed();

	static FGameplayTagContainer CreateValidSlotTags();

	void ActiveHighlight(const FGameplayTag _PartsTag);
	void DeActiveHighlight();


	FORCEINLINE bool GetEnabled() const { return bIsEnable; }
	FORCEINLINE void SetSlotTag(const FGameplayTag& Tag) { SlotTag = Tag; }
	FORCEINLINE TArray<UItemSlot*> GetEnableParts() { return EnableParts; }
	FORCEINLINE int32 GetItemID() { return ItemID; }
	
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsEnable; // 특정 조건 슬롯 활성화 때문인데. 지금은 안씀
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 ItemID = -1;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsSlotClick;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 SlotItemID = -1;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayTag SlotItemTag;
	
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_WeaponSlot;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Img_WeaponRender;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Normal;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Hovered;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Pressed;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Disable;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Pick;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_WeaponName;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_CurrentAmmo;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_TotalAmmo;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_AmmoType;

	UPROPERTY(meta = (BindWidget))
	UImage* Img_SlotFrame;

	UPROPERTY(meta = (BindWidget))
	UImage* Img_Ammo;

	UPROPERTY(meta = (BindWidget))
	UImage* Img_AmmoBG;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_SlotNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (AllowPrivateAccess=true))
	TArray<UTexture2D*> AmmoArray;

	UPROPERTY(editanywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<int32, UTexture2D*> WeaponArray;

	UPROPERTY(meta = (BindWidget))
	UItemSlot* Slot_Muzzle;

	UPROPERTY(meta = (BindWidget))
	UItemSlot* Slot_Grip;

	UPROPERTY(meta = (BindWidget))
	UItemSlot* Slot_Magazine;

	UPROPERTY(meta = (BindWidget))
	UItemSlot* Slot_Scope;

	UPROPERTY(meta = (BindWidget))
	UItemSlot* Slot_SideScope;

	UPROPERTY(meta = (BindWidget))
	UItemSlot* Slot_Stock;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf<UDragItemImage> DragItemWidgetClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	FGameplayTag WeaponTag;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	FGameplayTag SlotTag;

	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess))
	TArray<UItemSlot*> EnableParts;
};


