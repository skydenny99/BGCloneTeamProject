// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BaseInventoryComponent.h"
#include "Engine/DataTable.h"
#include "PlayerInventoryComponent.generated.h"

struct FInventoryItems;
struct FInventoryDataStruct;
struct FInventoryUI;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Local Struct
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBulletCountChanged, int32, NewCount);

/**
 * 플레이어 인벤토리 컴포넌트: BaseInventoryComponent를 상속받아 기능 구현
 */
UCLASS()
class DEDICATED_API UPlayerInventoryComponent : public UBaseInventoryComponent
{
	GENERATED_BODY()
public:
	// 컴포넌트 기본 값 설정
	UPlayerInventoryComponent();
	
	UPROPERTY(BlueprintAssignable, Category = "InventoryToHUD")
	FOnBulletCountChanged OnBulletCountChanged;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Local Array
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 데이터 테이블 참조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UDataTable* ItemDataTable;  // 아이템 정보를 담고 있는 데이터 테이블

	// 인벤토리에 저장된 아이템 정보 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventoryItems> InventoryItems;  // 데이터 테이블 구조체를 슬롯 형태로 사용
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventoryUI> InventoryUI;  // 데이터 테이블 구조체를 슬롯 형태로 사용

	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Local Variable
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//기본 무게
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	float DefualtWeight = 70.0f;  // 기본 소지 무게 70

	// 추가 무게
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	float EquipWeight = 0.0f;  // 장비 무게 관리

	//현제 최대 저장 가능 무게
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	float MaxWeight = 0.0f;  // 기본 + 장비 무게

	//현제 사용중인 무게
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	float CurrentWeight = 0.0f;  // 현제 사용중인 무게

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	FString DebugText;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Local Function
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 무게 업데이트
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateWeight(float InEquipWeight);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddUISlot(int32 InItemID,  int32 InItemCount);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AcquireDropItem(FInventoryItems& ItemList, int32 InCount, int32 RemainingCount ,int32& OutCount);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AcquireDropAmmo(FInventoryItems& ItemList, int32 InCount, int32 RemainingCount ,int32& OutCount);
public:
	//Pass Inventory Data Getter
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE float GetCurrentWeight() const { return CurrentWeight; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE float GetEquipWeight() const { return EquipWeight; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE float GetMaxWeight() const { return MaxWeight; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// virtual Function
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// 아이템 획득 함수
	virtual bool AcquireItem(int32 InItemID, int32 InCount, int32& OutRemainingCount) override; // 성공적으로 획득 시 true 반환

	// 아이템 버리기 함수
	virtual void DiscardItem(int32 InItemID, int32 InCount, int32& OutCount) override;

	// 아이템 사용 함수
	virtual bool UseItem(int32 InItemID) override;

	// 특정 아이템 보유 여부 확인 함수
	virtual bool HasItem(int32 InItemID, int32 InCount = 1) override;

	// 특정 탄약 보유 여부 확인 함수
	virtual bool ReloadBullet(EBulletType InBulletType, int32 InCount, int32& OutCount) override;

	// 특정 탄약 보유 수량 확인 요청
	virtual void HasBullet(EBulletType InBulletType, int32& OutCount) override;


	// 디버그 출력
	virtual void DebugInventoryPrint() override;

	UFUNCTION()
	TArray<FInventoryUI> GetInventoryData();
	
};

//한번더 수정 됨20250117





