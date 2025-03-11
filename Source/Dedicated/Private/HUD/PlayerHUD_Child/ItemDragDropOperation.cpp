// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/ItemDragDropOperation.h"
#include "HUD/UI/InventoryMainWidget.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"



void UItemDragDropOperation::Initialize(const int32 InitItemID, const int32 InitItemAmount,
                                        ABaseFieldItem* InitFieldItem, const bool _bIsInventory)
{
	this->ItemID = InitItemID;
	this->ItemAmount = InitItemAmount;
	this->FieldItem = InitFieldItem;
	bIsInventory = _bIsInventory;
}

void UItemDragDropOperation::Initialize(const int32 InitItemID, FGameplayTag& EquipWeaponTag, const bool _bIsInventory)

{
	WeaponTag = EquipWeaponTag;
	ItemID = InitItemID;
	bIsInventory = _bIsInventory;
}

void UItemDragDropOperation::Initialize(const int32 InitItemID, FGameplayTag& EquippedPartsTag,FGameplayTag& EquippedWeaponTag, const bool _bIsInventory)

{
	WeaponTag = EquippedWeaponTag;
	PartsTag = EquippedPartsTag;
	ItemID = InitItemID;
	bIsInventory = _bIsInventory;
}

