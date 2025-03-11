// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayTags.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

struct FGameplayTag;
class ABaseFieldItem;
/**
 *  드래그 후 타겟지점에 마우스 땔 시 해당 지점 UI에게 전달 되는 정보
 */
UCLASS()
class DEDICATED_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	/** 아이템의 경우 사용되는 함수 */
	void Initialize(const int32 InitItemID, const int32 InitItemAmount, ABaseFieldItem* InitFieldItem, const bool _bIsInventory = false);

	/** 무기의 경우 사용되는 함수 */
	void Initialize(const int32 InitItemID, FGameplayTag& EquipWeaponTag, const bool _bIsInventory = false);
	void Initialize(const int32 InitItemID, FGameplayTag& EquippedPartsTag,FGameplayTag& EquippedWeaponTag, const bool _bIsInventory = false);

	
	FORCEINLINE int32 GetItemID() const { return ItemID; }
	FORCEINLINE int32 GetItemAmount() const { return ItemAmount; }
	FORCEINLINE ABaseFieldItem* GetFieldItem() const { return FieldItem; }
	FORCEINLINE FGameplayTag& GetWeaponTag() {return WeaponTag;}
	FORCEINLINE FGameplayTag& GetPartsSlot() {return PartsTag;}
	FORCEINLINE bool GetIsInventory() const { return bIsInventory; }

	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DragDrop , meta = (AllowPrivateAccess = "true"))
	int32 ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DragDrop , meta = (AllowPrivateAccess = "true"))
	int32 ItemAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DragDrop , meta = (AllowPrivateAccess = "true"))
	ABaseFieldItem* FieldItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DragDrop , meta = (AllowPrivateAccess = "true"))
	FGameplayTag WeaponTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DragDrop , meta = (AllowPrivateAccess = "true"))
	FGameplayTag PartsTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DragDrop , meta = (AllowPrivateAccess = "true"))
	bool bIsInventory;


};
