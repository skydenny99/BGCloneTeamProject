// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"
#include "Characters/PubgBaseCharacter.h"
#include "Components/BaseWeaponManageComponent.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"
#include "Subsystem/PubgDataTableManageSubsystem.h"
#include "Weapon/WeaponPartsComponent.h"
#include "Weapon/Parts/WeaponPartsBase.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMeshComp");
	RootComponent = WeaponMeshComp;

	WeaponPartsComponent = CreateDefaultSubobject<UWeaponPartsComponent>(TEXT("WeaponPartsComponent"));
	WeaponPartsComponent->SetIsReplicated(true);
	bReplicates = true;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::GrantAbilities(APubgBaseCharacter* AbilityOwner)
{
	if (AbilityOwner == nullptr || WeaponGrantAbilities.IsEmpty()) return;
	GrantedAbilitySpecHandles = AbilityOwner->GrantAbilities(WeaponGrantAbilities);
}

void AWeaponBase::ClearAbilities(APubgBaseCharacter* AbilityOwner)
{
	if (AbilityOwner == nullptr || GrantedAbilitySpecHandles.IsEmpty()) return;
	AbilityOwner->ClearAbilities(GrantedAbilitySpecHandles);
}

void AWeaponBase::OnPullOut()
{
}

void AWeaponBase::OnPutAway()
{
	OnUpdateWeaponInfoDelegate.Clear();
}

bool AWeaponBase::IsEquippableParts(int32 InItemID) const
{
	if (HasAuthority())
	{
		if (const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(InItemID))
		{
			const AWeaponPartsBase* CDO = GetMutableDefault<AWeaponPartsBase>(ItemData->ItemActorClass.LoadSynchronous());
			return CDO->IsEquippable(WeaponPartsComponent->GetAvailablePartsSlot());
		}
	}
	return false;
}


void AWeaponBase::EquipWeaponParts(int32 InItemID)
{
	if (HasAuthority())
	{
		WeaponPartsComponent->EquipParts(InItemID);
	}
}

int32 AWeaponBase::UnequipWeaponParts(const FGameplayTag& InSlotTag)
{
	if (HasAuthority())
	{
		return WeaponPartsComponent->UnequipParts(InSlotTag);
	}
	return 0;
}

TArray<int32> AWeaponBase::GetAllWeaponPartsIDs()
{
	if (HasAuthority())
	{
		return WeaponPartsComponent->DiscardAllParts();
	}
	return TArray<int32>();
}


bool AWeaponBase::IsEquippableInSlot(const FGameplayTag& InTargetSlot) const
{
	return WeaponEquipSlot.HasTagExact(InTargetSlot);
}

AWeaponPartsBase* AWeaponBase::GetPartsInSlot(const FGameplayTag& InSlotTag, bool bGetDefault)
{
	return WeaponPartsComponent->GetPartsInSlot(InSlotTag, bGetDefault);
}

