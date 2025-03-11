// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/FieldItemManager.h"

#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"
#include "Item/BaseFieldItem.h"
#include "Subsystem/PubgDataTableManageSubsystem.h"


void AFieldItemManager::BeginPlay()
{
	Super::BeginPlay();
	if (GetNetMode() == NM_DedicatedServer)
	{
		InitWorldItems();
	}
}

// Sets default values
AFieldItemManager::AFieldItemManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AFieldItemManager::InitWorldItems()
{
	const FVector Location = FVector(2200, 700, 250); // debug spawn location
	const FVector AmmoLocation = FVector(2200, 700, 200); // debug spawn location
	SpawnItem(2, 1, Location);
	SpawnItem(151, 60, AmmoLocation);
}

void AFieldItemManager::SpawnItem(int32 ItemID, int32 Amount, const FVector& SpawnLocation)
{
	if (const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(ItemID); ItemData->FieldItemClass.IsNull() == false)
	{
		TSubclassOf<ABaseFieldItem> ItemClass = ItemData->FieldItemClass.LoadSynchronous();
		if (ItemClass)
		{
			ABaseFieldItem* Item = GetWorld()->SpawnActor<ABaseFieldItem>(ItemClass, SpawnLocation, FRotator::ZeroRotator);
			Item->SetAmount(Amount);
		}
	}
}
