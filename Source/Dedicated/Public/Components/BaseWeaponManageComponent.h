// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDebugHelper.h"
#include "BaseGameplayTags.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "BaseType/BaseEnumType.h"
#include "BaseWeaponManageComponent.generated.h"


class ABaseWeaponFieldItem;
class AProjectileBase;
class AWeaponBase;


// DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdatePlayerHUDAmmoSlate,EWeaponFireMode, FireMode, int32, CurrentAmmo, int32, TotalAmmo);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponEquipped);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponUnequipped);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponFireMode, EWeaponFireMode, FireMode);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateMaxAmmoCount, int32, Val);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEDICATED_API UBaseWeaponManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseWeaponManageComponent();

protected:
	UPROPERTY()
	AWeaponBase* CurrentEquippedWeapon = nullptr;
	
	FGameplayTag CurrentEquippedSlot = FGameplayTag();
	
	FGameplayTag LastEquippedSlot = FGameplayTag();

	UPROPERTY()
	TMap<FGameplayTag, AWeaponBase*> EquippedWeapons = {
		{BaseGameplayTags::Item_Slot_Primary, nullptr},
		{BaseGameplayTags::Item_Slot_Secondary, nullptr},
		{BaseGameplayTags::Item_Slot_SideArm, nullptr},
		{BaseGameplayTags::Item_Slot_Melee, nullptr},
		{BaseGameplayTags::Item_Slot_Throwable, nullptr},
		{BaseGameplayTags::Item_Slot_Punch, nullptr},
	};

	UPROPERTY()
	TMap<FGameplayTag, FName> SlotNameDict = {
		{BaseGameplayTags::Item_Slot_Primary, FName("backpack_01Socket")},
		{BaseGameplayTags::Item_Slot_Secondary, FName("backpack_02Socket")},
		{BaseGameplayTags::Item_Slot_SideArm, FName("slot_sidearm")},
		{BaseGameplayTags::Item_Slot_Melee, FName("slot_melee")},
		{BaseGameplayTags::Item_Slot_Throwable, FName("slot_throwable")}
	};

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeaponBase> PunchWeaponClass;
	UPROPERTY()
	AWeaponBase* PunchWeapon;

	virtual void BeginPlay() override;
	const FName& GetSlotNameByTag(const FGameplayTag& SlotTag) const {return SlotNameDict[SlotTag];}
	void ChangeAnimModeBySlot(const FGameplayTag& SlotTag);

	
public:
	FGameplayTag GetBlankEquippedSlot(const FGameplayTagContainer& EquippableSlots) const;
	UFUNCTION(BlueprintPure)
	AWeaponBase* GetWeaponInSlot(const FGameplayTag& SlotTag) const;
	// 나머지 getter도 추가 필요

	void EquipWeapon(AWeaponBase* InNewWeapon, const FGameplayTag& InEquippedSlot); // 무기 퀵슬롯 장착
	void UnequipWeapon(const FGameplayTag& InUnequippedSlot); // 무기 퀵슬롯 해제
	UFUNCTION(Server, Reliable)
	void Server_EquipWeapon(ABaseWeaponFieldItem* InFieldWeapon, const FGameplayTag& InEquippedSlot);
	UFUNCTION(Server, Reliable)
	void Server_UnequipWeapon(const FGameplayTag& InUnequippedSlot);
	UFUNCTION(Client, Reliable)
	void Client_UnequipWeapon(const FGameplayTag& InUnequippedSlot);

	UFUNCTION(BlueprintCallable)
	void PullOutWeapon(const FGameplayTag& InTargetSlot); // 무기 꺼내기
	UFUNCTION(BlueprintCallable)
	void PutAwayWeapon(const FGameplayTag& InTargetSlot); // 무기 집어넣기
	UFUNCTION(BlueprintCallable)
	void AttachWeaponOnSlot(AWeaponBase* InTargetWeapon, const FGameplayTag& InTargetSlot);
	UFUNCTION(BlueprintCallable)
	void AttachWeaponOnHand(const FGameplayTag& InTargetWeaponSlot, bool InIsLeft);


	UFUNCTION(BlueprintPure)
	AWeaponBase* GetCurrentWeapon() const {return CurrentEquippedWeapon;}
	UFUNCTION(BlueprintPure)
	FGameplayTag GetCurrentEquippedSlot() const {return CurrentEquippedSlot;}
	
	UFUNCTION(BlueprintPure)
	FGameplayTag GetLastEquippedSlot() const {return LastEquippedSlot;}

	UFUNCTION(BlueprintPure)
	EGunName GetGunName(const FGameplayTag& InTargetSlot);

	




	

	// HUD 업데이트
	// UFUNCTION(BlueprintCallable)
	// void UpdatePlayerHUDAmmoSlate(EWeaponFireMode FireMode, int32 NewCurrentAmmo, int32 NewTotalAmmo);
	//
	// UFUNCTION(BlueprintCallable)
	// void UpdateWeaponFireMode(EWeaponFireMode FireMode);

	UFUNCTION(BlueprintCallable)
	void UpdateMaxAmmoCount(int32 InItemID);

	UFUNCTION(Client, Reliable)
	void Client_UpdateInventory();

public:
	// HUD 이벤트 델리게이트
	// UPROPERTY(BlueprintAssignable)
	// FOnUpdatePlayerHUDAmmoSlate OnUpdatePlayerHUDAmmoSlate;
	//
	// UPROPERTY(BlueprintAssignable)
	// FOnWeaponEquipped OnWeaponEquipped;
	//
	// UPROPERTY(BlueprintAssignable)
	// FOnWeaponUnequipped OnWeaponUnequipped;
	//
	// UPROPERTY(BlueprintAssignable)
	// FOnWeaponFireMode OnWeaponFireMode;
	//
	// UPROPERTY(BlueprintAssignable)
	// FOnUpdateMaxAmmoCount OnUpdateMaxAmmoCount;	
};

