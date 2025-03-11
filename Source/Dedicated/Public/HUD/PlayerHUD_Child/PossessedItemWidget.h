// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "PossessedItemWidget.generated.h"

class UPossessWeaponSlotWidget;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class DEDICATED_API UPossessedItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void UpdateWeapon();
	void EquipWeapon(FGameplayTag _WeaponTag);

private:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_WeaponSlot;

	/** 슬롯 UI 클래스 (Blueprint에서 할당) */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPossessWeaponSlotWidget> PossessedWeaponWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, UPossessWeaponSlotWidget*> WeaponMap;
};
