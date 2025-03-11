// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "EquipManageComponent.generated.h"


struct FGameplayAbilitySpecHandle;
class AWeaponBase;
class ABaseGear;
struct FGameplayTag;
class ABaseGearFieldItem;

DECLARE_DELEGATE_OneParam(FOnCurrentWeaponUpdated, AWeaponBase*)

USTRUCT(BlueprintType)
struct FGearEquipInfo
{
	GENERATED_BODY()

	UPROPERTY()
	ABaseGear* Helmet;

	UPROPERTY()
	ABaseGear* Armor;

	UPROPERTY()
	ABaseGear* Backpack;
	
	ABaseGear* GetGearInSlot(const FGameplayTag& InSlotTag) const;
	void SetGearInSlot(ABaseGear* InGear, const FGameplayTag& InSlotTag);
};

USTRUCT(BlueprintType)
struct FWeaponEquipInfo
{
	GENERATED_BODY()

	UPROPERTY()
	AWeaponBase* PrimaryWeapon;

	UPROPERTY()
	AWeaponBase* SecondaryWeapon;

	UPROPERTY()
	AWeaponBase* SideArmWeapon;

	UPROPERTY()
	AWeaponBase* MeleeWeapon;

	UPROPERTY()
	AWeaponBase* ThrowableWeapon;

	UPROPERTY()
	AWeaponBase* PunchWeapon;
	
	AWeaponBase* GetWeaponInSlot(const FGameplayTag& InSlotTag) const;
	void SetWeaponInSlot(AWeaponBase* InWeapon, const FGameplayTag& InSlotTag);
	FGameplayTag GetSlotByWeapon(AWeaponBase* InWeapon) const;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEDICATED_API UEquipManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipManageComponent();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeaponBase> PunchClass;
	
	UPROPERTY(ReplicatedUsing=OnRep_CurrentGearEquipInfo)
	FGearEquipInfo CurrentGearEquipInfo;

	UPROPERTY(ReplicatedUsing=OnRep_CurrentWeaponEquipInfo)
	FWeaponEquipInfo CurrentWeaponEquipInfo;

	UPROPERTY(ReplicatedUsing=OnRep_CurrentWeaponSlot)
	FGameplayTag CurrentWeaponSlotTag;

	UPROPERTY(Replicated)
	FGameplayTag LastWeaponSlotTag;
	
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void AttachGearInSlot(ABaseGear* InGear);
	void AttachWeaponInHand(AWeaponBase* InWeapon, bool InIsLeft);
	void AttachWeaponInSlot(AWeaponBase* InWeapon, const FGameplayTag& InSlotTag);

	void DetachGear(ABaseGear* InGear);
	void DetachWeapon(AWeaponBase* InWeapon);
	
	void ChangeAnimModeBySlot(const FGameplayTag& SlotTag);

public:
	FOnCurrentWeaponUpdated OnCurrentWeaponUpdatedDelegate;
	FGameplayTag GetAvailableWeaponSlot(const FGameplayTagContainer& EquippableSlots) const;

	/// 장비를 장착함. 장비 장착 시 장비가 보유한 gameplay effect 적용
	/// @param InGear 장착할 장비
	void EquipGear(ABaseGear* InGear);

	/// 해당 슬롯의 장비를 착용 해제
	/// @param InEquipSlot 장착 해제할 슬롯
	/// @return 장착 해제한 장비
	ABaseGear* UnEquipGear(const FGameplayTag& InEquipSlot);
	
	/// 무기를 슬롯에 장착. 장착하는 슬롯은 비어있어야함. 만약 이미 있다면 Unequip으로 제거
	/// @param InWeapon 장착할 무기
	/// @param InEquipSlot 장착할 슬롯 태그
	void EquipWeapon(AWeaponBase* InWeapon, const FGameplayTag& InEquipSlot);

	/// 슬롯 간 무기 변경 시 사용. 
	/// @param InSourceSlot 무기 기존 슬롯
	/// @param InTargetSlot 무기 변경 슬롯
	void SwapWeapon(const FGameplayTag& InSourceSlot, const FGameplayTag& InTargetSlot);

	/// 무기를 장착 해제.
	/// @param InEquipSlot 장착 해제할 슬롯
	/// @return 장착 해제한 무기
	AWeaponBase* UnEquipWeapon(const FGameplayTag& InEquipSlot);

	/// 무기를 실제로 꺼냄. 무기가 보유한 Gameplay Ability를 부여하고, 장착 슬롯을 손으로 이동.
	/// @param InTargetSlot 꺼낼 무기 슬롯
	void PullOutWeapon(const FGameplayTag& InTargetSlot);

	/// 무기를 집어넣음. 무기가 부여한 Ability를 모두 clear하고, 장착 위치를 가방으로 이동.
	void PutAwayWeapon();

	UFUNCTION()
	void OnRep_CurrentWeaponSlot();

	UFUNCTION()
	void OnRep_CurrentWeaponEquipInfo();

	UFUNCTION()
	void OnRep_CurrentGearEquipInfo();

	UFUNCTION(Server, Reliable)
	void Server_SwapWeapon(const FGameplayTag& InSourceSlot, const FGameplayTag& InTargetSlot);
	

#pragma region Getter & Setter

	UFUNCTION(BlueprintPure)
	ABaseGear* GetGearInSlot(const FGameplayTag& InSlotTag) const {return CurrentGearEquipInfo.GetGearInSlot(InSlotTag);}
	UFUNCTION(BlueprintPure)
	AWeaponBase* GetWeaponInSlot(const FGameplayTag& InSlotTag) const {return CurrentWeaponEquipInfo.GetWeaponInSlot(InSlotTag);}
	UFUNCTION(BlueprintPure)
	AWeaponBase* GetCurrentWeapon() const { return CurrentWeaponEquipInfo.GetWeaponInSlot(CurrentWeaponSlotTag); }
	UFUNCTION(BlueprintPure)
	const FGameplayTag& GetCurrentWeaponSlotTag() const { return CurrentWeaponSlotTag; }
	UFUNCTION(BlueprintPure)
	const FGameplayTag& GetLastWeaponSlotTag() const { return LastWeaponSlotTag; }
#pragma endregion
};