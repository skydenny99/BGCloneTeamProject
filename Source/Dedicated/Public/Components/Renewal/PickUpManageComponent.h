// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BaseType/BaseEnumType.h"
#include "Components/ActorComponent.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "Gear/BaseGear.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"
#include "Item/BaseFieldItem.h"
#include "Subsystem/PubgDataTableManageSubsystem.h"
#include "PickUpManageComponent.generated.h"


struct FInventoryDataStruct;
class ABaseGear;
class AWeaponBase;
class ABaseWeaponFieldItem;
class ABaseGearFieldItem;
class ABaseFieldItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEDICATED_API UPickUpManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickUpManageComponent();

protected:
	UPROPERTY()
	TArray<ABaseFieldItem*> PickUpCandidates;

	void InternalPickUpItem(ABaseFieldItem* InFieldItem);
	void InternalPickUpWeapon(ABaseWeaponFieldItem* InFieldWeapon, const FGameplayTag& InSlotTag = FGameplayTag());
	void InternalPickUpGear(ABaseGearFieldItem* InFieldGear);
	void InternalPickUpParts(ABaseFieldItem* InFieldParts, const FGameplayTag& InWeaponSlotTag);

	void DiscardGear(ABaseGear* InGear);
	void DiscardWeapon(AWeaponBase* InWeapon);
	void DiscardWeaponParts(AWeaponBase* InWeapon, const FGameplayTag& InPartsSlotTag);

	//const FInventoryDataStruct* GetItemData(int32 InItemID) const;
	template<class ItemClass>
	ItemClass* SpawnFieldItemByItemID(int32 InItemID);
	template<class ItemClass>
	ItemClass* SpawnItemActorByItemID(int32 InItemID);
	
	EPickMode GetPickMode(const float LocationZ) const;
	
public:
	void AddPickUpCandidate(ABaseFieldItem* InFieldItem);
	void RemovePickUpCandidate(ABaseFieldItem* InFieldItem);
	const TArray<ABaseFieldItem*>& GetPickUpCandidates() const { return PickUpCandidates; };

	void PickUp(ABaseFieldItem* InFieldItem);
	
	/// 픽업 아이템 획득 시 일반적으로 사용.
	/// @param InFieldItem 획득한 픽업 아이템
	/// @param bNeedToPlayMontage 픽업 몽타주 재생 여부
	UFUNCTION(Server, Reliable)
	void Server_PickUp(ABaseFieldItem* InFieldItem, bool bNeedToPlayMontage);


	/// 픽업 아이템을 슬롯에 장착할 경우 사용.
	/// @param InFieldItem 장착할 픽업 아이템
	/// @param SlotTag 장착할 슬롯 태그
	UFUNCTION(Server, Reliable)
	void Server_PickUpWithSlot(ABaseFieldItem* InFieldItem, const FGameplayTag& SlotTag);

	/// 인벤토리에서 아이템을 버릴 때 사용.
	/// @param InItemID 버릴 아이템 ID
	/// @param InAmount 버릴 수량
	UFUNCTION(Server, Reliable)
	void Server_DiscardItem(int32 InItemID, int32 InAmount);

	/// 인벤토리에서 장비를 해제해서 버릴 때 사용.
	/// @param InSlotTag 장착 해제할 장비 슬롯
	UFUNCTION(Server, Reliable)
	void Server_DiscardGear(const FGameplayTag& InSlotTag);

	/// 인벤토리에서 무기를 버릴 때 사용.
	/// @param InSlotTag 장착 해제할 무기 슬롯
	UFUNCTION(Server, Reliable)
	void Server_DiscardWeapon(const FGameplayTag& InSlotTag);

	/// 파츠를 해제할 때 사용.
	/// @param InWeaponSlotTag 파츠를 해제할 무기 슬롯
	/// @param InPartsSlotTag 버릴 파츠의 슬롯
	UFUNCTION(Server, Reliable)
	void Server_DiscardWeaponParts(const FGameplayTag& InWeaponSlotTag, const FGameplayTag& InPartsSlotTag);

	/// 들고 있는 모든 아이템을 버릴 때 사용
	void DiscardAllItems();

};

template <class ItemClass>
ItemClass* UPickUpManageComponent::SpawnFieldItemByItemID(int32 InItemID)
{
	const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(InItemID);
	if (ItemData == nullptr) return nullptr;
	if (ItemData->FieldItemClass.IsNull() == false)
	{
		if (ItemClass* Item = GetWorld()->SpawnActor<ItemClass>(ItemData->FieldItemClass.LoadSynchronous(), GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation()))
		{
			return Item;
		}
	}
	return nullptr;
}

template <class ItemClass>
ItemClass* UPickUpManageComponent::SpawnItemActorByItemID(int32 InItemID)
{
	const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(InItemID);
	if (ItemData == nullptr) return nullptr;
	if (ItemData->ItemActorClass.IsNull() == false)
	{
		if (ItemClass* Item = GetWorld()->SpawnActor<ItemClass>(ItemData->ItemActorClass.LoadSynchronous(), GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation()))
		{
			return Item;
		}
	}
	return nullptr;
}

