// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BaseWeaponManageComponent.h"

#include "BaseDebugHelper.h"
#include "Characters/PubgBaseCharacter.h"
#include "Characters/PubgCharacter.h"
#include "Characters/PubgInventoryCharacter.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "HUD/HUDBase.h"
#include "HUD/UI/InventoryMainWidget.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"
#include "Item/BaseWeaponFieldItem.h"
#include "Subsystem/PubgDataTableManageSubsystem.h"
#include "Weapon/BaseGun.h"
#include "Weapon/WeaponBase.h"


// Sets default values for this component's properties
UBaseWeaponManageComponent::UBaseWeaponManageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UBaseWeaponManageComponent::BeginPlay()
{
	Super::BeginPlay();
	// init
	PunchWeapon = Cast<AWeaponBase>(GetWorld()->SpawnActor(PunchWeaponClass));
	EquipWeapon(PunchWeapon, BaseGameplayTags::Item_Slot_Punch);
	PullOutWeapon(BaseGameplayTags::Item_Slot_Punch);

}

void UBaseWeaponManageComponent::ChangeAnimModeBySlot(const FGameplayTag& SlotTag)
{
	if (SlotTag.IsValid() == false) return;
	APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(GetOwner());
	if (PlayerCharacter)
	{
		if (SlotTag == BaseGameplayTags::Item_Slot_Punch)
		{
			PlayerCharacter->ChangePlayerEquipMode(EPlayerActionMode::Unarmed);
		}
		else if (SlotTag == BaseGameplayTags::Item_Slot_Melee)
		{
			PlayerCharacter->ChangePlayerEquipMode(EPlayerActionMode::Melee);
		}
		else if (SlotTag == BaseGameplayTags::Item_Slot_Throwable)
		{
			PlayerCharacter->ChangePlayerEquipMode(EPlayerActionMode::Grenade);
		}
		else
		{
			PlayerCharacter->ChangePlayerEquipMode(EPlayerActionMode::Rifle);
		}
	}
		
}



FGameplayTag UBaseWeaponManageComponent::GetBlankEquippedSlot(const FGameplayTagContainer& EquippableSlots) const
{
	if (EquippableSlots.IsEmpty()) return FGameplayTag();
	for (auto& Slot : EquippableSlots)
	{
		if (GetWeaponInSlot(Slot) == nullptr) return Slot;
	}
	return FGameplayTag();
}

AWeaponBase* UBaseWeaponManageComponent::GetWeaponInSlot(const FGameplayTag& SlotTag) const
{
	if (SlotTag.IsValid() == false || SlotTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Punch)) return nullptr;
	return EquippedWeapons[SlotTag];
}

// deprecated method, Please use Server_EquipWeapon
void UBaseWeaponManageComponent::EquipWeapon(AWeaponBase* InNewWeapon, const FGameplayTag& InEquippedSlot)
{
	if (InNewWeapon == nullptr)	return;
	UnequipWeapon(InEquippedSlot);
	EquippedWeapons[InEquippedSlot] = InNewWeapon;
	APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(GetOwner());
	//InNewWeapon->OnEquip(PlayerCharacter, InEquippedSlot);
}

// deprecated method, Please use Server_UnequipWeapon
void UBaseWeaponManageComponent::UnequipWeapon(const FGameplayTag& InUnequippedSlot)
{
	if (EquippedWeapons[InUnequippedSlot])
	{
		//EquippedWeapons[InUnequippedSlot]->OnUnequip();
	}
	EquippedWeapons[InUnequippedSlot] = nullptr;
}

void UBaseWeaponManageComponent::Server_EquipWeapon_Implementation(ABaseWeaponFieldItem* InFieldWeapon,
	const FGameplayTag& InEquippedSlot)
{
	if (InFieldWeapon == nullptr || InEquippedSlot.IsValid() == false) return;
	if(AWeaponBase* SpawnedWeapon = GetWorld()->SpawnActor<AWeaponBase>(InFieldWeapon->GetWeaponClass()))
	{
		InFieldWeapon->OverrideWeaopnSpec(SpawnedWeapon);
		Server_UnequipWeapon(InEquippedSlot);
		EquippedWeapons[InEquippedSlot] = SpawnedWeapon;
		APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(GetOwner());
		//SpawnedWeapon->OnEquip(PlayerCharacter, InEquippedSlot);
		AttachWeaponOnSlot(SpawnedWeapon, InEquippedSlot);
		InFieldWeapon->Destroy();

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle,[this]()
		{
			Client_UpdateInventory();
			Debug::Print("Call Lambda Function",FColor::Red);
			Debug::Print("Call Lambda Function",FColor::Red);
			Debug::Print("Call Lambda Function",FColor::Red);
		},0.2f,false);
	}
}

void UBaseWeaponManageComponent::Server_UnequipWeapon_Implementation(const FGameplayTag& InUnequippedSlot)
{
	if (InUnequippedSlot.IsValid() == false || EquippedWeapons[InUnequippedSlot] == nullptr) return;
	if (CurrentEquippedSlot == InUnequippedSlot) // 서버 측에서 어빌리티 관리
	{
		PutAwayWeapon(InUnequippedSlot);
		PullOutWeapon(BaseGameplayTags::Item_Slot_Punch);
	}
	//EquippedWeapons[InUnequippedSlot]->OnUnequip();
	EquippedWeapons[InUnequippedSlot]->Destroy();
	EquippedWeapons[InUnequippedSlot] = nullptr;
	Client_UnequipWeapon(InUnequippedSlot);
}
void UBaseWeaponManageComponent::Client_UnequipWeapon_Implementation(const FGameplayTag& InUnequippedSlot)
{
	if (CurrentEquippedSlot == InUnequippedSlot)
	{
		PullOutWeapon(BaseGameplayTags::Item_Slot_Punch);
	}
	EquippedWeapons[InUnequippedSlot] = nullptr;
}


void UBaseWeaponManageComponent::PullOutWeapon(const FGameplayTag& InTargetSlot)
{
	if (EquippedWeapons[InTargetSlot] == nullptr) return;
	APubgCharacter* Owner = Cast<APubgCharacter>(GetOwner());
	if (Owner)
	{
		EquippedWeapons[InTargetSlot]->GrantAbilities(Owner);
		// if (ABaseGun* Gun = Cast<ABaseGun>(EquippedWeapons[InTargetSlot]))
		// {
		// 	if (Gun->GetGunSpec().SpeedPenalty)
		// 	{
		// 		UBaseAbilitySystemComponent* ASC = Owner->GetBaseAbilitySystemComponent();
		// 		ASC->ApplyGameplayEffectToSelf(Gun->GetGunSpec().SpeedPenalty.GetDefaultObject(), 1.f, ASC->MakeEffectContext());	
		// 	}
		// }
	}
	CurrentEquippedWeapon = EquippedWeapons[InTargetSlot];
	CurrentEquippedSlot = InTargetSlot;
	ChangeAnimModeBySlot(CurrentEquippedSlot);
	// HUD 초기화
	//CurrentEquippedWeapon->WeaponEquipInit();
	Owner->ChangePlayerGunType(GetGunName(CurrentEquippedSlot));
	LastEquippedSlot = CurrentEquippedSlot;
	Owner->GetPubgInventoryCharacter()->SetCurrentWeapon(CurrentEquippedSlot);
	Owner->Client_InventoryChangeEquip();
	//Debug::Print(FString::Printf(TEXT("InTarget Slot %s"),*InTargetSlot.ToString()));
}

void UBaseWeaponManageComponent::PutAwayWeapon(const FGameplayTag& InTargetSlot)
{
	if (EquippedWeapons[InTargetSlot] == nullptr) return;
	APubgBaseCharacter* Owner = Cast<APubgBaseCharacter>(GetOwner());
	if (Owner && EquippedWeapons[InTargetSlot])
	{
		EquippedWeapons[InTargetSlot]->ClearAbilities(Owner);
		// if (Cast<ABaseGun>(EquippedWeapons[InTargetSlot]))
		// {
		// 	UBaseAbilitySystemComponent* ASC = Owner->GetBaseAbilitySystemComponent();
		// 	ASC->RemoveActiveEffects(FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(
		// 								FGameplayTagContainer(BaseGameplayTags::State_Debuff_WeaponSpeedPenalty)
		// 								));
		// }
	}
	CurrentEquippedWeapon = nullptr;
	CurrentEquippedSlot = FGameplayTag();
}


EGunName UBaseWeaponManageComponent::GetGunName(const FGameplayTag& InTargetSlot)
{
	// if (!EquippedWeapons.Contains(InTargetSlot) || !EquippedWeapons[InTargetSlot])
	// {
	// 	return EWeaponType::Default;
	// }
	if (ABaseGun* Gun = Cast<ABaseGun>(EquippedWeapons[InTargetSlot]))
	{
		return Gun->GetGunSpec().GunName;
	}
	return EGunName::Default;
}
//
// void UBaseWeaponManageComponent::UpdatePlayerHUDAmmoSlate(EWeaponFireMode FireMode, int32 NewCurrentAmmo,
// 	int32 NewTotalAmmo)
// {
// 	Debug::Print(FString::Printf(TEXT("UBaseWeaponManageComponent -- UpdatePlayerHUDAmmoSlate")));
// 	OnUpdatePlayerHUDAmmoSlate.Broadcast(FireMode, NewCurrentAmmo, NewTotalAmmo);
// }
//
// void UBaseWeaponManageComponent::UpdateWeaponFireMode(EWeaponFireMode FireMode)
// {
// 	OnWeaponFireMode.Broadcast(FireMode);
// }

void UBaseWeaponManageComponent::UpdateMaxAmmoCount(int32 InItemID)
{
	if (APubgBaseCharacter* Owner = Cast<APubgBaseCharacter>(GetOwner()))
	{
		if (ABaseGun* Gun = Cast<ABaseGun>(GetCurrentWeapon()))
		{
			if (const FInventoryDataStruct* ItemInfo = UBaseFunctionLibrary::GetItemData(InItemID))
			{
				if (ItemInfo->ItemType == EItemType::Ammobox)
				{
					// Debug::Print(TEXT("UBaseWeaponManageComponent ----- UpdateMaxAmmoCount"));
					// int32 OutVal = 0;
					// Owner->GetInventoryComponent()->HasBullet(Gun->GetGunSpec().UsedBullet, OutVal);
					// OnUpdateMaxAmmoCount.Broadcast(OutVal);
				}
			}
		}
	}
	
}

void UBaseWeaponManageComponent::Client_UpdateInventory_Implementation()
{
	if (APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(GetOwner()))
	{
		PlayerCharacter->GetHUDBase()->GetInventoryMainWidget()->UpdateWeapon();
		Debug::Print("WeaponManager :: Call Getter Access Inventory Weapon");
	}
	else
	{
		Debug::Print("WeaponManager :: Cast Failed GetOwner");
	}
}


void UBaseWeaponManageComponent::AttachWeaponOnSlot(AWeaponBase* InTargetWeapon,
                                                    const FGameplayTag& InTargetSlot)
{
	if (InTargetSlot.IsValid() == false || InTargetSlot.MatchesTagExact(BaseGameplayTags::Item_Slot_Punch)) return;
	if (USkeletalMeshComponent* Mesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		InTargetWeapon->AttachToComponent(Mesh, AttachmentRules, GetSlotNameByTag(InTargetSlot));
	}
}

void UBaseWeaponManageComponent::AttachWeaponOnHand(const FGameplayTag& InTargetWeaponSlot, bool InIsLeft)
{
	if (InTargetWeaponSlot.IsValid() == false || InTargetWeaponSlot.MatchesTagExact(BaseGameplayTags::Item_Slot_Punch)) return;
	if (USkeletalMeshComponent* Mesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		GetWeaponInSlot(InTargetWeaponSlot)->AttachToComponent(Mesh, AttachmentRules, InIsLeft ? FName(TEXT("item_l")) : FName(TEXT("item_r")));
	}
}