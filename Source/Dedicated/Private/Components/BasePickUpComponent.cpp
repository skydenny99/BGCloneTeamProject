// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BasePickUpComponent.h"

#include "BaseDebugHelper.h"
#include "Characters/PubgCharacter.h"
#include "Components/BaseInventoryComponent.h"
#include "Components/BaseWeaponManageComponent.h"
#include "Components/PlayerInventoryComponent.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "HUD/HUDBase.h"
#include "HUD/UI/InventoryMainWidget.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"
#include "Item/BaseFieldItem.h"
#include "Item/BaseWeaponFieldItem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Subsystem/PubgDataTableManageSubsystem.h"


// Sets default values for this component's properties
UBasePickUpComponent::UBasePickUpComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UBasePickUpComponent::BeginPlay()
{
	Super::BeginPlay();
	PubgCharacter = Cast<APubgCharacter>(GetOwner());
	if (PubgCharacter == nullptr)
	{
		Debug::PrintError("Owner of PickUP component is not APubgCharacter");
	}
}

void UBasePickUpComponent::AddPickUpCandidate(ABaseFieldItem* PickUpItem)
{
	PickUpCandidates.AddUnique(PickUpItem);
}

void UBasePickUpComponent::RemovePickUpCandidate(ABaseFieldItem* PickUpItem)
{
	PickUpCandidates.Remove(PickUpItem);
}

void UBasePickUpComponent::PickUp(ABaseFieldItem* InPickUpItem)
{
	Server_PickUp(InPickUpItem, true);
}

void UBasePickUpComponent::Server_PickUp_Implementation(ABaseFieldItem* InPickUpItem, bool NeedToPlayMontage)
{
	Debug::Print("Server TryToPickUp_Implementation");
	if (InPickUpItem == nullptr) return;
	/*const float AllowDistSquare = 31500.0f;
	if (FVector::DistSquared2D(InPickUpItem->GetActorLocation(), GetOwner()->GetActorLocation()) > AllowDistSquare)
	{
		Debug::Print("Pickup Distance : ",FVector::DistSquared2D(InPickUpItem->GetActorLocation(), GetOwner()->GetActorLocation()));
		return;
	}*/
	Debug::Print(FString::Printf(TEXT("%s pick up - ID: %d, Amount: %d"), *GetOwner()->GetActorNameOrLabel(), InPickUpItem->GetItemID(), InPickUpItem->GetItemAmount()));

	if (NeedToPlayMontage)
	{
		if (PubgCharacter)
		{
			Debug::Print(FString::Printf(TEXT("PickupMode : %s"), *UEnum::GetDisplayValueAsText(GetPickMode(InPickUpItem->GetActorLocation().Z)).ToString()));
			PubgCharacter->PlayPickUpMontage(GetPickMode(InPickUpItem->GetActorLocation().Z));
		}
	}
	if (const FInventoryDataStruct* ItemInfo = UBaseFunctionLibrary::GetItemData(InPickUpItem->GetItemID()))
	{
		if (ItemInfo->ItembIsStorable)
		{
			InternalPickUpItem(InPickUpItem);
		}
		else
		{
			if (ABaseWeaponFieldItem* WeaponFieldItem = Cast<ABaseWeaponFieldItem>(InPickUpItem)) // weapon 이라면 weapon manage
			{
				InternalPickUpWeapon(WeaponFieldItem);
			}

			// TODO: GearManage
		}
	}
}


void UBasePickUpComponent::Server_DiscardPickUp_Implementation(int32 ItemID, int32 ItemAmount)
{
	if (PubgCharacter)
	{
		int DiscardAmount = 0;
		// PubgCharacter->GetInventoryComponent()->DiscardItem(ItemID, ItemAmount, DiscardAmount);
		// PubgCharacter->GetInventoryComponent()->Client_DiscardItem(ItemID, ItemAmount);
		DiscardAmount = ItemAmount - DiscardAmount;
		if (DiscardAmount > 0)
		{
			if (const FInventoryDataStruct* ItemInfo = UBaseFunctionLibrary::GetItemData(ItemID))
			{
				if (ItemInfo->FieldItemClass.IsNull() == false)
				{
					/*if (ABaseFieldItem* FieldItem = GetWorld()->SpawnActor<ABaseFieldItem>(ItemInfo.FieldItemClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation()))
					{
						//AddPickUpCandidate(FieldItem);
						FieldItem->SetAmount(DiscardAmount);
					}*/

					if (ABaseFieldItem* FieldItem = GetWorld()->SpawnActorDeferred<ABaseFieldItem>(ItemInfo->FieldItemClass.LoadSynchronous(),  FTransform(GetOwner()->GetActorRotation(),GetOwner()->GetActorLocation())))
					{
						if (FieldItem)
						{
							FieldItem->SetWorldSpawned(true);
							FieldItem->SetAmount(DiscardAmount);

							UGameplayStatics::FinishSpawningActor(FieldItem,FTransform(GetOwner()->GetActorRotation(),GetOwner()->GetActorLocation()));
						}
					}
				}
			}
		}
	}
}


void UBasePickUpComponent::Server_DiscardWeapon_Implementation(FGameplayTag EquipTag)
{
	// if (PubgCharacter)
	// {
	// 	AWeaponBase* Weapon = PubgCharacter->GetWeaponManageComponent()->GetWeaponInSlot(EquipTag);
	// 	if (Weapon == nullptr) return;
	// 	//int32 UnequipWeaponID = Weapon->GetItemID();
	// 	int32 UnequipWeaponID = Cast<ABaseGun>(Weapon)->GetGunSpec().ItemID;
	// 	if (UItemDataSubsystem* DB = GetWorld()->GetSubsystem<UItemDataSubsystem>())
	// 	{
	// 		Debug::Print("UnequipWeaponID : ", UnequipWeaponID);
	// 		const FInventoryDataStruct& ItemInfo = DB->GetItemData(UnequipWeaponID);
	// 		if (ItemInfo.FieldItemClass.Get())
	// 		{
	// 			// 기존에 있던 함수.
	// 			/*if(ABaseWeaponFieldItem* SpawnedWeapon = GetWorld()->SpawnActor<ABaseWeaponFieldItem>(ItemInfo.FieldItemClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation()))
	// 			{
	// 				SpawnedWeapon->InitWeaponSpec(Weapon);
	// 				AddPickUpCandidate(SpawnedWeapon);
	// 				//PubgCharacter->GetHUDBase()->GetInventoryMainWidget()->UpdateAroundItem();
	// 				PubgCharacter->GetWeaponManageComponent()->Server_UnequipWeapon(EquipTag);
	// 			}*/
	//
	// 			// 생성시 BeginPlay에 근처에 있는 플레이어에게 자기 어필 활성화
	// 			if (ABaseWeaponFieldItem* SpawnedWeapon = GetWorld()->SpawnActorDeferred<ABaseWeaponFieldItem>(ItemInfo.FieldItemClass.Get(),  FTransform(GetOwner()->GetActorRotation(),GetOwner()->GetActorLocation())))
	// 			{
	// 				if (SpawnedWeapon)
	// 				{
	// 					SpawnedWeapon->SetWorldSpawned(true);
	//
	// 					//기존에 있던 함수
	// 					SpawnedWeapon->InitWeaponSpec(Weapon);
	// 					//AddPickUpCandidate(SpawnedWeapon);
	// 					PubgCharacter->GetWeaponManageComponent()->Server_UnequipWeapon(EquipTag);
	//
	// 					UGameplayStatics::FinishSpawningActor(SpawnedWeapon,FTransform(GetOwner()->GetActorRotation(),GetOwner()->GetActorLocation()));
	//
	//
	// 					FTimerHandle TimerHandle;
	// 					GetWorld()->GetTimerManager().SetTimer(TimerHandle,[this]()
	// 					{
	// 						Client_WeaponUpdate();
	// 						Debug::Print("Call Lambda Function",FColor::Red);
	// 					},0.2f,false);
	// 				}
	// 			}
	// 		}
	// 	}
	// }
}

void UBasePickUpComponent::Client_AddPickUpCandidate_Implementation(ABaseFieldItem* PickUpItem)
{
	if (PickUpItem)
	{
		AddPickUpCandidate(PickUpItem);
		Debug::Print("Client_AddPickUpCandidate_Implementation : Success");
	}
	else
	{
		Debug::Print("Client_AddPickUpCandidate_Implementation : Failed");
	}
	
}

void UBasePickUpComponent::Debug_ShowPickUpCandidates()
{
	for (int32 Index = 0; Index < PickUpCandidates.Num(); Index++)
	{
		Debug::Print(FString::Printf(TEXT("Item: %s, Index: %d"), *PickUpCandidates[Index]->GetActorNameOrLabel(), Index));
	}
}

void UBasePickUpComponent::Debug_PickUpCandidateByIndex(int32 InIndex)
{
	if (PickUpCandidates.IsValidIndex(InIndex) == false) return;
	PickUp(PickUpCandidates[InIndex]);
}

void UBasePickUpComponent::Client_UpdateWidget_Implementation()
{
	if (APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(GetOwner()))
	{
		PlayerCharacter->GetHUDBase()->GetInventoryMainWidget()->UpdateInventory();
		PlayerCharacter->GetHUDBase()->GetInventoryMainWidget()->UpdateAroundItem();
		PlayerCharacter->GetHUDBase()->GetInventoryMainWidget()->UpdateWeapon();
		Debug::Print("Client_UpdateWidget_Implementation : Success");
	}
	else
		Debug::Print("Client_UpdateWidget_Implementation : Failed");
}

void UBasePickUpComponent::InternalPickUpItem(ABaseFieldItem* InPickUpItem)
{
	if (PubgCharacter)
	{
		int32 RemainAmount = 0;
		// if (PubgCharacter->GetInventoryComponent()->AcquireItem(InPickUpItem->GetItemID(), InPickUpItem->GetItemAmount(), RemainAmount))
		// {
		// 	// acquire success, send rpc to client
		// 	PubgCharacter->GetInventoryComponent()->Client_AcquireItem(InPickUpItem->GetItemID(), InPickUpItem->GetItemAmount());
		// 	if (RemainAmount == 0)
		// 	{
		// 		InPickUpItem->Destroy();
		// 	}
		// 	else
		// 	{
		// 		InPickUpItem->SetAmount(RemainAmount);
		// 	}
		//
		// 	Client_UpdateWidget();
		// }
	}
}

void UBasePickUpComponent::InternalPickUpWeapon(ABaseWeaponFieldItem* InPickUpItem)
{
	// if (PubgCharacter && InPickUpItem)
	// {
	// 	FGameplayTag EquipTag = PubgCharacter->GetWeaponManageComponent()->GetBlankEquippedSlot(InPickUpItem->GetEquippableTags());
	// 	if (EquipTag.IsValid()) // 빈칸이 있을 경우
	// 	{
	// 		PubgCharacter->GetWeaponManageComponent()->Server_EquipWeapon(InPickUpItem, EquipTag);
	// 	}
	// 	else // 빈칸이 없을 경우
	// 	{
	// 		if (PubgCharacter->GetWeaponManageComponent()->GetWeaponInSlot(BaseGameplayTags::Item_Slot_Secondary))
	// 		{
	// 			Server_DiscardWeapon(BaseGameplayTags::Item_Slot_Secondary);
	// 		}
	// 		PubgCharacter->GetWeaponManageComponent()->Server_EquipWeapon(InPickUpItem, BaseGameplayTags::Item_Slot_Secondary);
	// 	}
	// }
}

void UBasePickUpComponent::InternalPickUpArmor(ABaseFieldItem* InPickUpItem)
{
	InPickUpItem->Destroy();
}

EPickMode UBasePickUpComponent::GetPickMode(const float LocationZ) const
{
	const float OwnerZ = GetOwner()->GetActorLocation().Z;
	if (UKismetMathLibrary::Abs( OwnerZ - LocationZ) < 20.f)
		return EPickMode::Middle;
	if (OwnerZ > LocationZ)
		return EPickMode::Low;
	return EPickMode::High;
}

void UBasePickUpComponent::Client_WeaponUpdate_Implementation()
{
	if (APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(GetOwner()))
	{
		PlayerCharacter->GetHUDBase()->GetInventoryMainWidget()->UpdateWeapon();
		Debug::Print("Client_WeaponUpdate_Implementation : Success");
	}
}
