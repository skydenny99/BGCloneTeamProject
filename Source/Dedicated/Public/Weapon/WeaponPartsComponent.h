// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "WeaponPartsComponent.generated.h"

class AWeaponPartsBase;

USTRUCT(BlueprintType)
struct FWeaponPartsInfo
{
	GENERATED_BODY()

	UPROPERTY()
	AWeaponPartsBase* MagazineParts;
	
	UPROPERTY()
	AWeaponPartsBase* SightParts;
	
	UPROPERTY()
	AWeaponPartsBase* GripParts;
	
	UPROPERTY()
	AWeaponPartsBase* MuzzleParts;
	
	UPROPERTY()
	AWeaponPartsBase* StockParts;

	void SetPartsInSlot(const FGameplayTag& InSlotTag, AWeaponPartsBase* InWeaponParts);
	AWeaponPartsBase* GetPartsInSlot(const FGameplayTag& InSlotTag);
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEDICATED_API UWeaponPartsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponPartsComponent();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(ReplicatedUsing=OnRep_UpdateCurrentParts)
	FWeaponPartsInfo CurrentWeaponPartsInfo;
	
	UPROPERTY(Replicated)
	FWeaponPartsInfo DefaultWeaponPartsInfo;
	
	UPROPERTY(EditDefaultsOnly, meta=(Categories="Item.Parts"))
	FGameplayTagContainer AvailablePartsSlot;

	UPROPERTY(EditDefaultsOnly)
	TArray<int32> DefaultPartsIDSetting;

	void AttachPartsInSlot(const FGameplayTag& InSlotTag, AWeaponPartsBase* InWeaponParts);
	void DetachParts(AWeaponPartsBase* InWeaponParts);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
public:
	/// 총기에 기본 장착될 파츠를 장착
	/// @param InItemID Default 슬롯에 장착할 아이템
	void EquipDefaultParts(int32 InItemID);

	/// 외부에서 얻은 파츠를 장착, default 슬롯을 비활성화함
	/// @param InItemID Current 슬롯에 장착할 아이템
	/// @return 장착 성공 시 true
	bool EquipParts(int32 InItemID);

	/// 슬롯 태그에 해당하는 파츠를 장착해제
	/// @param InSlotTag 버릴 파츠의 슬롯 태그
	/// @return 버려진 파츠의 item ID
	int32 UnequipParts(const FGameplayTag& InSlotTag);

	/// 모든 Current 파츠를 제거 (Default는 건드리지 않음), 총기를 버릴 때 사용
	/// @return 장착 해제된 모든 파츠의 item ID
	TArray<int32> DiscardAllParts();

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	UFUNCTION(BlueprintPure)
	AWeaponPartsBase* GetPartsInSlot(const FGameplayTag& InSlotTag, bool bGetDefault);

	FORCEINLINE const FGameplayTagContainer& GetAvailablePartsSlot() const { return AvailablePartsSlot; }
	
	FString GetSightInfo(float& OutSightFov);
	
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetMuzzleInfo();

	void OnFppZoomIn();
	void OnFppZoomOut();

	UFUNCTION()
	void OnRep_UpdateCurrentParts();
};
