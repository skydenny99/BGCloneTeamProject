// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InventoryManageComponent.generated.h"

struct FGameplayTag;

USTRUCT(BlueprintType)
struct FInventoryListItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 ItemID;

	UPROPERTY()
	int32 ItemAmount;
};

USTRUCT(BlueprintType)
struct FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaSerializeInfo);
	void AddItem(int32 InItemID, int32 InItemAmount);
	void UpdateItem(int32 InItemID, int32 InItemDeltaAmount);
	bool HasItem(int32 InItemID, int32 InItemAmount) const;
	int32 GetItemAmount(int32 InItemID) const;

	TArray<FInventoryListItem>& GetItemsRef() {return Items;}

protected:
	UPROPERTY()
	TArray<FInventoryListItem> Items;
};

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum {WithNetDeltaSerializer = true};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEDICATED_API UInventoryManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryManageComponent();

protected:
	UPROPERTY(ReplicatedUsing=OnRep_UpdateItem)
	FInventoryList InventoryList;

	UPROPERTY(Replicated)
	float CurrentWeight;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	int32 CalculateAvailableAmount(int32 InItemID, int32 InItemAmount);
	void UpdateWeight(int32 InItemID, int32 InItemAmount);
	float GetMaxWeight() const;

public:
	/// 아이템을 인벤토리에 추가
	/// @param InItemID 획득할 아이템 ID
	/// @param InAmount 획득할 아이템 개수
	/// @param OutRemainAmount 필드에 남은 아이템 개수
	/// @return 인벤토리에 추가 완료 시 true, 획득하지 못했으면 false
	bool AcquireItem(int32 InItemID, int32 InAmount, int32& OutRemainAmount);

	/// 인벤토리 내의 아이템을 버림. 만약 InAmount보다 실제 소지량이 적다면 실제 소지량만큼만 버림.
	/// @param InItemID 버릴 아이템 ID
	/// @param InAmount 버릴 아이템 개수
	/// @return 실제로 버려진 아이템 개수
	int32 DiscardItem(int32 InItemID, int32 InAmount);

	/// 인벤토리의 모든 아이템을 삭제
	void ClearItems();

	/// 인벤토리 내의 소지 아이템을 수량만큼 사용
	/// @param InItemID 사용할 아이템 ID
	/// @param InAmount 사용할 아이템 개수
	void UseItem(int32 InItemID, int32 InAmount);
	
	UFUNCTION(Server, Reliable)
	void Server_UseItem(int32 InItemID, int32 InAmount);

	UFUNCTION(Server, Reliable)
	void Server_EquipParts(int32 InPartsItemID, const FGameplayTag& InWeaponSlotTag);

	bool HasItem(int32 InItemID, int32 InAmount = 1) const;
	int32 GetItemAmount(int32 InItemID) const;

	FORCEINLINE float GetCurrentWeight() const {return CurrentWeight;}
	FORCEINLINE TArray<FInventoryListItem>& GetItemsRef() {return InventoryList.GetItemsRef();}

	UFUNCTION()
	void OnRep_UpdateItem();
};
