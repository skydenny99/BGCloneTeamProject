// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Renewal/EquipManageComponent.h"

#include "BaseGameplayTags.h"
#include "Characters/PubgBaseCharacter.h"
#include "Characters/PubgCharacter.h"
#include "Characters/PubgInventoryCharacter.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "Gear/BaseGear.h"
#include "HUD/HUDBase.h"
#include "HUD/PlayerHUD/PlayerHUD.h"
#include "HUD/UI/InventoryMainWidget.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/BaseGun.h"
#include "Weapon/WeaponBase.h"



ABaseGear* FGearEquipInfo::GetGearInSlot(const FGameplayTag& InSlotTag) const
{
	if (InSlotTag.IsValid() == false) return nullptr;
	if (InSlotTag.MatchesTagExact(BaseGameplayTags::Item_Gear_Armor))
	{
		return Armor;
	}

	if (InSlotTag.MatchesTagExact(BaseGameplayTags::Item_Gear_Backpack))
	{
		return Backpack;
	}

	if (InSlotTag.MatchesTagExact(BaseGameplayTags::Item_Gear_Helmet))
	{
		return Helmet;
	}

	return nullptr;
}

void FGearEquipInfo::SetGearInSlot(ABaseGear* InGear, const FGameplayTag& InSlotTag)
{
	if (InSlotTag.IsValid() == false) return;
	if (InSlotTag.MatchesTagExact(BaseGameplayTags::Item_Gear_Armor))
	{
		Armor = InGear;
	}

	if (InSlotTag.MatchesTagExact(BaseGameplayTags::Item_Gear_Backpack))
	{
		Backpack = InGear;
	}

	if (InSlotTag.MatchesTagExact(BaseGameplayTags::Item_Gear_Helmet))
	{
		Helmet = InGear;
	}
}

AWeaponBase* FWeaponEquipInfo::GetWeaponInSlot(const FGameplayTag& InSlotTag) const
{
	if (InSlotTag.IsValid() == false) return nullptr;
	if (InSlotTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Primary))
	{
		return PrimaryWeapon;
	}
		
	if (InSlotTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Secondary))
	{
		return SecondaryWeapon;
	}
		
	if (InSlotTag.MatchesTagExact(BaseGameplayTags::Item_Slot_SideArm))
	{
		return SideArmWeapon;
	}
		
	if (InSlotTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Melee))
	{
		return MeleeWeapon;
	}
		
	if (InSlotTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Throwable))
	{
		return ThrowableWeapon;
	}
		
	if (InSlotTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Punch))
	{
		return PunchWeapon;
	}
	
	return nullptr;
}

void FWeaponEquipInfo::SetWeaponInSlot(AWeaponBase* InWeapon, const FGameplayTag& InSlotTag)
{
	if (InSlotTag.IsValid() == false) return;
	if (InSlotTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Primary))
	{
		PrimaryWeapon = InWeapon;
	}
		
	if (InSlotTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Secondary))
	{
		SecondaryWeapon = InWeapon;
	}
		
	if (InSlotTag.MatchesTagExact(BaseGameplayTags::Item_Slot_SideArm))
	{
		SideArmWeapon = InWeapon;
	}
		
	if (InSlotTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Melee))
	{
		MeleeWeapon = InWeapon;
	}
		
	if (InSlotTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Throwable))
	{
		ThrowableWeapon = InWeapon;
	}
		
	if (InSlotTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Punch))
	{
		PunchWeapon = InWeapon;
	}
}

FGameplayTag FWeaponEquipInfo::GetSlotByWeapon(AWeaponBase* InWeapon) const
{
	if (IsValid(InWeapon) == false) return FGameplayTag();;
	if (InWeapon == PrimaryWeapon)
	{
		return BaseGameplayTags::Item_Slot_Primary;
	}
		
	if (InWeapon == SecondaryWeapon)
	{
		return BaseGameplayTags::Item_Slot_Secondary;
	}
	
	if (InWeapon == SideArmWeapon)
	{
		return BaseGameplayTags::Item_Slot_SideArm;
	}
	
	if (InWeapon == MeleeWeapon)
	{
		return BaseGameplayTags::Item_Slot_Melee;
	}
	
	if (InWeapon == ThrowableWeapon)
	{
		return BaseGameplayTags::Item_Slot_Throwable;
	}
	
	if (InWeapon == PunchWeapon)
	{
		return BaseGameplayTags::Item_Slot_Punch;
	}
		
	return FGameplayTag();
}

// Sets default values for this component's properties
UEquipManageComponent::UEquipManageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	CurrentWeaponSlotTag = BaseGameplayTags::Item_Slot_Punch;
	// ...
}

void UEquipManageComponent::BeginPlay()
{
	Super::BeginPlay();
	if (PunchClass)
	{
		if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
		{
			EquipWeapon(PunchClass.GetDefaultObject(), BaseGameplayTags::Item_Slot_Punch);
		}
	}
}

void UEquipManageComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// DOREPLIFETIME_CONDITION(UEquipManageComponent, CurrentWeaponEquipInfo, COND_AutonomousOnly);
	DOREPLIFETIME(UEquipManageComponent, CurrentWeaponEquipInfo);
	DOREPLIFETIME_CONDITION(UEquipManageComponent, CurrentGearEquipInfo, COND_AutonomousOnly);
	// DOREPLIFETIME_CONDITION(UEquipManageComponent, CurrentWeaponSlotTag, COND_AutonomousOnly);
	DOREPLIFETIME(UEquipManageComponent, CurrentWeaponSlotTag);
	DOREPLIFETIME_CONDITION(UEquipManageComponent, LastWeaponSlotTag, COND_AutonomousOnly);
}

void UEquipManageComponent::AttachGearInSlot(ABaseGear* InGear)
{
	if (IsValid(InGear) == false) return;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		if (USkeletalMeshComponent* Mesh = GetOwner<APubgBaseCharacter>()->GetMesh())
		{
			const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
			InGear->AttachToComponent(Mesh, AttachmentRules, FName("root"));
				
			// if (USkeletalMeshComponent* GearMesh = InGear->FindComponentByClass<USkeletalMeshComponent>())
			// {
			// 	GearMesh->SetLeaderPoseComponent(Mesh);
			// }
		}
	}
}

void UEquipManageComponent::DetachGear(ABaseGear* InGear)
{
	if (IsValid(InGear) == false) return;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		const FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
		InGear->DetachFromActor(DetachmentRules);
	}
}

void UEquipManageComponent::AttachWeaponInHand(AWeaponBase* InWeapon, bool InIsLeft)
{
	if (IsValid(InWeapon) == false || InWeapon == CurrentWeaponEquipInfo.PunchWeapon) return;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		if (USkeletalMeshComponent* Mesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
		{
			const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
			InWeapon->AttachToComponent(Mesh, AttachmentRules, InIsLeft ? FName(TEXT("item_l")) : FName(TEXT("item_r")));
		}
	}
}

void UEquipManageComponent::AttachWeaponInSlot(AWeaponBase* InWeapon, const FGameplayTag& InSlotTag)
{
	if (IsValid(InWeapon) == false || InSlotTag.IsValid() == false || InSlotTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Punch)) return;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		const TMap<FGameplayTag, FName> SlotNameDict = {
			{BaseGameplayTags::Item_Slot_Primary, FName("backpack_01Socket")},
			{BaseGameplayTags::Item_Slot_Secondary, FName("backpack_02Socket")},
			{BaseGameplayTags::Item_Slot_SideArm, FName("slot_sidearm")},
			{BaseGameplayTags::Item_Slot_Melee, FName("slot_melee")},
			{BaseGameplayTags::Item_Slot_Throwable, FName("slot_throwable")}
		};

		const FName& SlotName = SlotNameDict[InSlotTag];
		if (SlotName.IsNone())
		{
			Debug::PrintError("Weapon Slot Name Error");
		}
		else
		{
			if (USkeletalMeshComponent* Mesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
			{
				const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
				InWeapon->AttachToComponent(Mesh, AttachmentRules, SlotName);
			}
		}
	}
	
}

void UEquipManageComponent::DetachWeapon(AWeaponBase* InWeapon)
{
	if (IsValid(InWeapon) == false) return;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		const FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
		InWeapon->DetachFromActor(DetachmentRules);
	}
}

void UEquipManageComponent::ChangeAnimModeBySlot(const FGameplayTag& SlotTag)
{
	if (SlotTag.IsValid() == false) return;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		if (APubgCharacter* PlayerCharacter = GetOwner<APubgCharacter>())
		{
			if (SlotTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Punch) )
			{
				PlayerCharacter->ChangePlayerEquipMode(EPlayerActionMode::Unarmed);
			}
			else if (SlotTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Melee))
			{
				PlayerCharacter->ChangePlayerEquipMode(EPlayerActionMode::Melee);
			}
			else if (SlotTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Throwable))
			{
				PlayerCharacter->ChangePlayerEquipMode(EPlayerActionMode::Grenade);
			}
			else
			{
				PlayerCharacter->ChangePlayerEquipMode(EPlayerActionMode::Rifle);
			}

			if (ABaseGun* Gun = Cast<ABaseGun>(GetCurrentWeapon()))
			{
				PlayerCharacter->ChangePlayerGunType(Gun->GetGunSpec().GunName);
			}
		}
	}
	
}

void UEquipManageComponent::EquipGear(ABaseGear* InGear)
{
	if (IsValid(InGear) == false) return;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		UBaseAbilitySystemComponent* ASC = UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(GetOwner());
		if (ASC)
		{
			ASC->ApplyGameplayEffectToSelf(InGear->GetGearGameplayEffectClass().GetDefaultObject(), InGear->GetGearLevel(), ASC->MakeEffectContext());
		}
		CurrentGearEquipInfo.SetGearInSlot(InGear, InGear->GetGearType());
		AttachGearInSlot(InGear);
	}
}

ABaseGear* UEquipManageComponent::UnEquipGear(const FGameplayTag& InEquipSlot)
{
	ABaseGear* Gear = CurrentGearEquipInfo.GetGearInSlot(InEquipSlot);
	if (IsValid(Gear) == false) return nullptr;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		if (UBaseAbilitySystemComponent* ASC = UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(GetOwner()))
		{
			ASC->RemoveActiveEffectsWithTags(Gear->GetGearType().GetSingleTagContainer());
		}
		CurrentGearEquipInfo.SetGearInSlot(nullptr, Gear->GetGearType());
		DetachGear(Gear);
		return Gear;
	}
	
	return nullptr;
}

FGameplayTag UEquipManageComponent::GetAvailableWeaponSlot(const FGameplayTagContainer& EquippableSlots) const
{
	if (EquippableSlots.IsEmpty()) return FGameplayTag();
	for (auto& Slot : EquippableSlots)
	{
		if (CurrentWeaponEquipInfo.GetWeaponInSlot(Slot) == nullptr) return Slot;
	}
	return EquippableSlots.GetByIndex(0);
}


void UEquipManageComponent::EquipWeapon(AWeaponBase* InWeapon, const FGameplayTag& InEquipSlot)
{
	if (IsValid(InWeapon) == false || InEquipSlot.IsValid() == false) return;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		if (IsValid(CurrentWeaponEquipInfo.GetWeaponInSlot(InEquipSlot)))
		{
			Debug::PrintError(FString::Printf(TEXT("%s already equipped"), *InEquipSlot.ToString()));
			return;
		}
		CurrentWeaponEquipInfo.SetWeaponInSlot(InWeapon, InEquipSlot);
		if (CurrentWeaponSlotTag.MatchesTagExact(InEquipSlot))
		{
			PullOutWeapon(InEquipSlot);
		}
		else
		{
			AttachWeaponInSlot(InWeapon, InEquipSlot);
		}
	}
}

void UEquipManageComponent::SwapWeapon(const FGameplayTag& InSourceSlot, const FGameplayTag& InTargetSlot)
{
	if (InSourceSlot.IsValid() == false || InTargetSlot.IsValid() == false || InSourceSlot.MatchesTagExact(InTargetSlot)) return;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		AWeaponBase* SourceSlotWeapon = CurrentWeaponEquipInfo.GetWeaponInSlot(InSourceSlot);
		AWeaponBase* TargetSlotWeapon = CurrentWeaponEquipInfo.GetWeaponInSlot(InTargetSlot);
		APubgBaseCharacter* OwningCharacter = GetOwner<APubgBaseCharacter>();
		
		if (IsValid(SourceSlotWeapon) == false || SourceSlotWeapon->IsEquippableInSlot(InTargetSlot) == false) return;
		const bool bCurrentSlotIsTarget = CurrentWeaponSlotTag.MatchesTagExact(InTargetSlot);
		const bool bCurrentSlotIsSource = CurrentWeaponSlotTag.MatchesTagExact(InSourceSlot);

		if (IsValid(TargetSlotWeapon))
		{
			if (TargetSlotWeapon->IsEquippableInSlot(InSourceSlot) == false) return;
			if (bCurrentSlotIsSource)
			{
				AttachWeaponInHand(TargetSlotWeapon, false);
			}
			else
			{
				AttachWeaponInSlot(TargetSlotWeapon, InSourceSlot);
			}
		}
		
		if (bCurrentSlotIsTarget)
		{
			AttachWeaponInHand(SourceSlotWeapon, false);
		}
		else
		{
			AttachWeaponInSlot(SourceSlotWeapon, InTargetSlot);
		}
		
		// Swapping Ability
		if (bCurrentSlotIsSource || bCurrentSlotIsTarget)
		{
			OwningCharacter->ClearAbilities(GrantedAbilitySpecHandles);
			GrantedAbilitySpecHandles.Empty();

			const TArray<TSubclassOf<UBaseGameplayAbility>>& WeaponAbilities =
				bCurrentSlotIsSource ? TargetSlotWeapon->GetAbilitiesGrantByWeapon() : SourceSlotWeapon->GetAbilitiesGrantByWeapon();
			GrantedAbilitySpecHandles = OwningCharacter->GrantAbilities(WeaponAbilities);
		}
		
		CurrentWeaponEquipInfo.SetWeaponInSlot(TargetSlotWeapon, InSourceSlot);
		CurrentWeaponEquipInfo.SetWeaponInSlot(SourceSlotWeapon, InTargetSlot);
		ChangeAnimModeBySlot(CurrentWeaponSlotTag);
	}
}

AWeaponBase* UEquipManageComponent::UnEquipWeapon(const FGameplayTag& InEquipSlot)
{
	AWeaponBase* UnEquippedWeapon = CurrentWeaponEquipInfo.GetWeaponInSlot(InEquipSlot);
	if (IsValid(UnEquippedWeapon) == false || InEquipSlot.MatchesTagExact(BaseGameplayTags::Item_Slot_Punch)) return nullptr;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		CurrentWeaponEquipInfo.SetWeaponInSlot(nullptr, InEquipSlot);
		DetachWeapon(UnEquippedWeapon);
		if (CurrentWeaponSlotTag.MatchesTagExact(InEquipSlot))
		{
			if (APubgBaseCharacter* OwningCharacter = GetOwner<APubgBaseCharacter>())
			{
				OwningCharacter->ClearAbilities(GrantedAbilitySpecHandles);
				GrantedAbilitySpecHandles.Empty();
			}
			PullOutWeapon(BaseGameplayTags::Item_Slot_Punch);
			LastWeaponSlotTag = FGameplayTag::EmptyTag;
		}
		return UnEquippedWeapon;
	}
	return nullptr;
}

void UEquipManageComponent::PullOutWeapon(const FGameplayTag& InTargetSlot)
{
	AWeaponBase* TargetWeapon = CurrentWeaponEquipInfo.GetWeaponInSlot(InTargetSlot);
	if (IsValid(TargetWeapon) == false) return;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		if (APubgBaseCharacter* OwningCharacter = GetOwner<APubgBaseCharacter>())
		{
			const TArray<TSubclassOf<UBaseGameplayAbility>>& WeaponAbilities = TargetWeapon->GetAbilitiesGrantByWeapon();
			GrantedAbilitySpecHandles = OwningCharacter->GrantAbilities(WeaponAbilities);
		}
		AttachWeaponInHand(TargetWeapon, false); // temp: 우측 손에 장착
		ChangeAnimModeBySlot(InTargetSlot);
		CurrentWeaponSlotTag = InTargetSlot;
		
		if (InTargetSlot.MatchesTagExact(BaseGameplayTags::Item_Slot_Punch) == false)
		{
			LastWeaponSlotTag = InTargetSlot;
		}
		OnCurrentWeaponUpdatedDelegate.ExecuteIfBound(CurrentWeaponEquipInfo.GetWeaponInSlot(CurrentWeaponSlotTag));
	}
}

void UEquipManageComponent::PutAwayWeapon()
{
	AWeaponBase* TargetWeapon = CurrentWeaponEquipInfo.GetWeaponInSlot(CurrentWeaponSlotTag);
	if (IsValid(TargetWeapon) == false) return;
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		if (APubgBaseCharacter* OwningCharacter = GetOwner<APubgBaseCharacter>())
		{
			OwningCharacter->ClearAbilities(GrantedAbilitySpecHandles);
				GrantedAbilitySpecHandles.Empty();
		}
		AttachWeaponInSlot(TargetWeapon, CurrentWeaponSlotTag);
		CurrentWeaponSlotTag = FGameplayTag();
	}
}

void UEquipManageComponent::OnRep_CurrentWeaponSlot()
{
	if (APubgCharacter* OwningCharacter = Cast<APubgCharacter>(GetOwner()))
	{
		if (OwningCharacter->GetHUDBase())
		{
			OwningCharacter->GetHUDBase()->GetPlayerHUD()->GetPossessedItemWidget()->EquipWeapon(CurrentWeaponSlotTag);
		}
		else
		{
			Debug::Print("OnRep_CurrentWeaponSlot :: GetHUDBase is Failed");
		}
	}
	else
	{
		Debug::Print("OnRep_CurrentWeaponSlot :: OwningCharacter is Failed");
	}
	
	OnCurrentWeaponUpdatedDelegate.ExecuteIfBound(CurrentWeaponEquipInfo.GetWeaponInSlot(CurrentWeaponSlotTag));
}

void UEquipManageComponent::OnRep_CurrentWeaponEquipInfo()
{
	if (APubgCharacter* OwningCharacter = Cast<APubgCharacter>(GetOwner()))
	{
		if (OwningCharacter->GetHUDBase())
		{
			OwningCharacter->GetHUDBase()->GetInventoryMainWidget()->UpdateWeapon();
			OwningCharacter->GetHUDBase()->GetPlayerHUD()->UpdatePossessWeapon();
		}
		else
		{
			Debug::Print("OnRep_CurrentWeaponEquipInfo :: GetHUDBase is Failed");
		}
	}
	else
	{
		Debug::Print("OnRep_CurrentWeaponEquipInfo :: OwningCharacter is Failed");
	}
	OnCurrentWeaponUpdatedDelegate.ExecuteIfBound(CurrentWeaponEquipInfo.GetWeaponInSlot(CurrentWeaponSlotTag));
}

void UEquipManageComponent::OnRep_CurrentGearEquipInfo()
{
	if (APubgCharacter* OwningCharacter = Cast<APubgCharacter>(GetOwner()))
	{
		if (OwningCharacter->GetHUDBase())
		{
			OwningCharacter->GetHUDBase()->GetInventoryMainWidget()->UpdateGear();
			OwningCharacter->GetPubgInventoryCharacter()->UpdateGear();
		}
		else
		{
			Debug::Print("OnRep_CurrentWeaponEquipInfo :: GetHUDBase is Failed");
		}
	}
	else
	{
		Debug::Print("OnRep_CurrentWeaponEquipInfo :: OwningCharacter is Failed");
	}
}

void UEquipManageComponent::Server_SwapWeapon_Implementation(const FGameplayTag& InSourceSlot,
	const FGameplayTag& InTargetSlot)
{
	SwapWeapon(InSourceSlot, InTargetSlot);
}
