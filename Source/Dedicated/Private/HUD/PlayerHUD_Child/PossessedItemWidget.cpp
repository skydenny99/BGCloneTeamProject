// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/PossessedItemWidget.h"

#include "BaseDebugHelper.h"
#include "BaseGameplayTags.h"
#include "Characters/PubgCharacter.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "HUD/PlayerHUD_Child/PossessedItemSlot/PossessWeaponSlotWidget.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"
#include "Subsystem/PubgDataTableManageSubsystem.h"
#include "Weapon/WeaponBase.h"


void UPossessedItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	int32 ChildCount = VB_WeaponSlot->GetChildrenCount();

	for (int32 i = 0; i < ChildCount; i++)
	{
		if (UPossessWeaponSlotWidget* SlotWidget = Cast<UPossessWeaponSlotWidget>(VB_WeaponSlot->GetChildAt(i)))
		{
			int32 DisplayNumber = (ChildCount) - i;
			Debug::Print(FString::Printf(TEXT("UPossessedItemWidget :: %d"), DisplayNumber));
			SlotWidget->SetupNumber(DisplayNumber);


			switch (i)
			{
			case 0:
				WeaponMap.Add(BaseGameplayTags::Item_Slot_Throwable, SlotWidget);
				break;
			case 1:
				WeaponMap.Add(BaseGameplayTags::Item_Slot_Melee, SlotWidget);
				break;
			case 2:
				WeaponMap.Add(BaseGameplayTags::Item_Slot_SideArm, SlotWidget);
				break;
			case 3:
				WeaponMap.Add(BaseGameplayTags::Item_Slot_Secondary, SlotWidget);
				break;
			case 4:
				WeaponMap.Add(BaseGameplayTags::Item_Slot_Primary, SlotWidget);
				break;
			default:
				break;
			}
		}
	}
}

void UPossessedItemWidget::UpdateWeapon()
{
	APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter) return;

	UEquipManageComponent* EquipManager = PlayerCharacter->GetEquipManageComponent();
	if (!EquipManager) return;
	
	for (const TPair<FGameplayTag, UPossessWeaponSlotWidget*>& TempPair : WeaponMap)
	{
		FGameplayTag TempTag = TempPair.Key;
		UPossessWeaponSlotWidget* TempWidget = TempPair.Value;

		if (AWeaponBase* TempWeaponBase = EquipManager->GetWeaponInSlot(TempTag))
		{
			const FGunSpec* GunData = UBaseFunctionLibrary::GetGunData(TempWeaponBase->GetItemID());
			TempWidget->SetupImage(GunData->HUDWeaponImage);
			/*TempWidget->ToggleAlpha(true);
			TempWidget->SetRenderOpacity(5.f);*/
			TempWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}

void UPossessedItemWidget::EquipWeapon(FGameplayTag _WeaponTag)
{
	for (const TPair<FGameplayTag, UPossessWeaponSlotWidget*>& TempPair : WeaponMap)
	{
		FGameplayTag TempTag = TempPair.Key;
		UPossessWeaponSlotWidget* TempWidget = TempPair.Value;

		if (TempTag == _WeaponTag)
		{
			if (TempWidget->GetRenderOpacity() != 5.f)
				TempWidget->SetRenderOpacity(5.f);
		}
		else
		{
			if (TempWidget->GetRenderOpacity() != 1.f)
				TempWidget->SetRenderOpacity(1.f);
		}
	}
}