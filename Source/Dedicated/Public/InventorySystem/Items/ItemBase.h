// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"
#include "InventorySystem/TestFramework/InventoryTestCharacter.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	//////////////////////////////////////////////////////////////////////////////////////
	// PROPERTIES & VARIABLES
	///
	
	/** 아이템 고유 ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemId")
	int32 ItemID; // 예: 1은 AKM, 251은 수류탄

	/** 검색용 아이템 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemId")
	FString ItemName; // 예: "akm" (내부 시스템에서 사용되는 식별 이름)

	/** 아이템 타입 (EItemType 열거형을 사용) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemType")
	EItemType ItemType; // 아이템 타입 추가

	/** UI 표시용 아이템 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemUi")
	FString DisplayName; // 예: "AKM" (유저에게 표시되는 이름)

	/** 아이템 설명 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemUi")
	FString Description; // 예: "표준형 돌격 소총으로, 화력이 보통 수준입니다."
	
	/** 인벤토리에서 아이템의 무게 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemNumeric")
	float Weight; // 예: 구급상자 10, 12게이지 총알 1.25

	/** 해당 아이템의 스택 가능 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemNumeric")
	bool bIsStackable; // 예: 무기 스택 False, 회복템 스택 True

	/** 해당 아이템의 최대 스택 수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemNumeric", meta = (EditCondition = "bIsStackable"))
	int32 MaxStackCount; // 예: 구급상자 스택수 무한, 12게이지 총알 60발 제한

	//아이템 스텍 카운트 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemNumeric")
	int32 StackCount;
	
	/** 해당 아이템의 가방에 보관 가능 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemNumeric")
	bool bIsStorable; // 예: 무기 보관 False, 회복템 보관 True

	/** UI에 사용되는 아이콘 이미지 경로 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemAssetPath")
	FString IconPath; // 예: "/Game/UI/Icons/AKM.png"

	/** 월드에서 사용될Mesh 에셋 경로 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemAssetPath")
	FString MeshPath; 
	
	//////////////////////////////////////////////////////////////////////////////////////
	// FUNCTIONS
	///
	UItemBase();

	UFUNCTION(Category = "ItemData")
	UItemBase* CreateItemCopy();
	
	//총 무게
	UFUNCTION(Category = "ItemData")
	FORCEINLINE float GetItemStackWeight() const {return StackCount * Weight; };
	//게당 무게
	UFUNCTION(Category = "ItemData")
	FORCEINLINE float GetItemSingleWeight() const {return Weight; };
	//최대 스텍 확인
	UFUNCTION(Category = "ItemData")
	FORCEINLINE bool IsFullItemStack() const {return StackCount == MaxStackCount; };

	//스택 카운트 업데이트 용
	UFUNCTION(Category = "ItemData")
	void SetStackCount(const int32 NewStackCount);
	
	//아이템 사용시 플레이어와 연결
	//UFUNCTION(Category = "ItemData")
	//virtual void Use(AInventoryTestCharacter* Character);
	
protected:
	//아이디 일치 확인 
	bool operator == (const int32& OtherIdD)const
	{
		return ItemID == OtherIdD;
	}
	//////////////////////////////////////////////////////////////////////////////////////
	// PROPERTIES & VARIABLES
	///


	
	//////////////////////////////////////////////////////////////////////////////////////
	// FUNCTIONS
	///

	
};
