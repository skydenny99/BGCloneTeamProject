// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseType/BaseEnumType.h"
#include "Components/ActorComponent.h"
#include "BaseInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEDICATED_API UBaseInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseInventoryComponent();
	

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TMap<EBulletType, int32> CurrentBulletCount;
	

public:
	// 아이템 획득 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool AcquireItem(int32 InItemID, int32 InCount ,int32& OutRemainingCount); // 성공적으로 획득 시 true 반환

	UFUNCTION(Client, Reliable)
	void Client_AcquireItem(int32 InItemID, int32 InCount);
	
	// 아이템 버리기 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void DiscardItem(int32 InItemID, int32 InCount,int32& OutCount);

	UFUNCTION(Client, Reliable)
	void Client_DiscardItem(int32 InItemID, int32 InCount);
	

	// 아이템 사용 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool UseItem(int32 InItemID);

	// 특정 아이템 보유 여부 확인 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool HasItem(int32 InItemID, int32 InCount = 1);

	// 특정 탄약 장전 요청
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool ReloadBullet(EBulletType InBulletType,int32 InCount , int32& OutCount);

	// 특정 탄약 보유 수량 확인 요청
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void HasBullet(EBulletType InBulletType, int32& OutCount);

	// 
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void DebugInventoryPrint();
};


