// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/UI/InventoryMainWidget.h"

#include "AudioMixerBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Characters/PubgCharacter.h"
#include "Characters/PubgInventoryCharacter.h"
#include "Components/BasePickUpComponent.h"
#include "Components/BaseWeaponManageComponent.h"
#include "HUD/UI/InventorySlotWidget.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/VerticalBox.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "Components/Renewal/InventoryManageComponent.h"
#include "Components/Renewal/PickUpManageComponent.h"
#include "HUD/PlayerHUD_Child/GearWidget.h"
#include "HUD/PlayerHUD_Child/ItemSlot.h"
#include "HUD/PlayerHUD_Child/ItemTooltipWidget.h"
#include "HUD/PlayerHUD_Child/ScrollBarWidget.h"
#include "HUD/PlayerHUD_Child/WeaponSlot.h"
#include "Item/BaseFieldItem.h"
#include "Weapon/BaseGun.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/Parts/WeaponPartsBase.h"


void UInventoryMainWidget::EnableHighLightWeaponSlot(const FGameplayTag _ItemTag)
{
	if (_ItemTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Primary) ||  _ItemTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Secondary) || _ItemTag.MatchesTag(BaseGameplayTags::Item_Parts))
	{
		UWeaponSlot* PrimaryWidget = Cast<UWeaponSlot>(VB_Primary->GetChildAt(0));
		UWeaponSlot* SecondaryWidget = Cast<UWeaponSlot>(VB_Secondary->GetChildAt(0));

		if (_ItemTag.MatchesTag(BaseGameplayTags::Item_Parts))
		{
			PrimaryWidget->ActiveHighlight(_ItemTag);
			SecondaryWidget->ActiveHighlight(_ItemTag);
		}
		else
		{
			PrimaryWidget->SlotPressed();
			SecondaryWidget->SlotPressed();	
		}
		return;
	}

	if (_ItemTag.MatchesTag(BaseGameplayTags::Item_Gear))
	{
		WBP_Gear->ActiveHighLight(_ItemTag);
		return;
	}

	if (_ItemTag.MatchesTag(BaseGameplayTags::Item_Parts))
	{
		UWeaponSlot* PrimarySlotWidget = Cast<UWeaponSlot>(VB_Primary->GetChildAt(0));
		UWeaponSlot* SecondarySlotWidget = Cast<UWeaponSlot>(VB_Secondary->GetChildAt(0));

		TArray<UWeaponSlot*> WeaponSlots = {PrimarySlotWidget, SecondarySlotWidget};

		for (UWeaponSlot* WeaponSlot : WeaponSlots)
		{
			if (WeaponSlot->GetItemID() != -1)
			{
				WeaponSlot->ActiveHighlight(_ItemTag);
			}
		}
	}
}

void UInventoryMainWidget::DisableHighLightWeaponSlot(const FGameplayTag _ItemTag)
{
	if (_ItemTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Primary) ||  _ItemTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Secondary)
		|| _ItemTag.MatchesTag(BaseGameplayTags::Item_Parts))
	{
		UWeaponSlot* PrimaryWidget = Cast<UWeaponSlot>(VB_Primary->GetChildAt(0));
		UWeaponSlot* SecondaryWidget = Cast<UWeaponSlot>(VB_Secondary->GetChildAt(0));

		if (_ItemTag.MatchesTag(BaseGameplayTags::Item_Parts))
		{
			PrimaryWidget->DeActiveHighlight();
			SecondaryWidget->DeActiveHighlight();
		}
		else
		{
			PrimaryWidget->OnSlotUnHovered();
			SecondaryWidget->OnSlotUnHovered();
		}
	}

	if (_ItemTag.MatchesTag(BaseGameplayTags::Item_Gear))
	{
		WBP_Gear->DeActiveHighLight();
	}
}
void UInventoryMainWidget::DisableHighLightWeaponSlot()
{
	UWeaponSlot* PrimaryWidget = Cast<UWeaponSlot>(VB_Primary->GetChildAt(0));
	UWeaponSlot* SecondaryWidget = Cast<UWeaponSlot>(VB_Secondary->GetChildAt(0));

	PrimaryWidget->OnSlotUnHovered();
	PrimaryWidget->DeActiveHighlight();
	
	SecondaryWidget->OnSlotUnHovered();
	SecondaryWidget->DeActiveHighlight();

	WBP_Gear->DeActiveHighLight();

	
}



void UInventoryMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (WBP_ItemTooltip && bActiveTooltipBox)
	{
		FVector2D MousePos;
		if (GetOwningPlayer()->GetMousePosition(MousePos.X, MousePos.Y))
		{
			float DPI = UWidgetLayoutLibrary::GetViewportScale(this);
			FVector2D AdjustedMousePos = MousePos / DPI;

			// 마우스 포인트 옆에서 출력되도록
			AdjustedMousePos += FVector2D(10);
			WBP_ItemTooltip->SetRenderTranslation(AdjustedMousePos);
		}
	}

	
	
}

void UInventoryMainWidget::UpdateInventory()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController) return;

	APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter) return;

	UInventoryManageComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();
	if (!InventoryComponent) return;

	const TArray<FInventoryListItem>& InventoryData = InventoryComponent->GetItemsRef(); // 인벤토리 데이터 가져오기
	WBP_InventoryScrollBox->GetScrollBox()->ClearChildren(); // 기존 슬롯 삭제

	for (const FInventoryListItem& Item : InventoryData)
	{
		if (Item.ItemAmount != 0)
		{
			if (UInventorySlotWidget* SlotWidget = CreateWidget<UInventorySlotWidget>(this, InventorySlotClass))
			{
				SlotWidget->SetupSlot(Item);
				SlotWidget->SetbIsInventory(true);
				WBP_InventoryScrollBox->GetScrollBox()->AddChild(SlotWidget);
			}
		}
		
	}
	Debug::Print("Inven Count ", InventoryData.Num());
	Debug::Print("Complete UpdateInventory!");
}

void UInventoryMainWidget::UpdateAroundItem()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController) return;

	APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter) return;

	UPickUpManageComponent* PickupComponent = PlayerCharacter->GetPickUpManageComponent();
	if (!PickupComponent) return;


	// ABaseFieldItem에는 Id,Amount만 있음
	TArray<ABaseFieldItem*> CandidateItems = PickupComponent->GetPickUpCandidates();

	// 다지우고 다시채운다? 쓰읍..  나중에 필요한것만 업데이트 하도록 변경예정
	WBP_AroundScrollBox->GetScrollBox()->ClearChildren();

	for (const auto& CandidateItem : CandidateItems)
	{
		if (UInventorySlotWidget* SlotWidget = CreateWidget<UInventorySlotWidget>(this, InventorySlotClass))
		{
			if (SlotWidget)
			{
				SlotWidget->SetupSlot(CandidateItem);
				SlotWidget->SetbIsInventory(false);
				WBP_AroundScrollBox->GetScrollBox()->AddChild(SlotWidget);
			}
		}
	}
	Debug::Print("Around Count ", CandidateItems.Num());
	Debug::Print("Complete AroundUpdate!");
}

void UInventoryMainWidget::UpdateWeapon()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController)
	{
		Debug::Print("UpdateWeapon :: PlayerController is nullptr");
		return;
	}

	APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter) 
	{
		Debug::Print("UpdateWeapon :: PlayerCharacter is nullptr");
		return;
	}

	UPickUpManageComponent* PickupComponent = PlayerCharacter->GetPickUpManageComponent();
	if (!PickupComponent)
	{
		Debug::Print("UpdateWeapon :: PickupComponent is nullptr");
		return;
	}

	APubgInventoryCharacter* InventoryCharacter = PlayerCharacter->GetPubgInventoryCharacter();
	if (!InventoryCharacter)
	{
		Debug::Print("UpdateWeapon :: InventoryCharacter is nullptr");
		return;
	}
	
	UInventoryManageComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();
	if (!InventoryComponent) return;

	VB_Primary->ClearChildren();
	VB_Secondary->ClearChildren();

	int32 PrimaryWeaponID = 0;
	int32 PrimaryCurrentAmmo = 0;
	int32 PrimaryTotalAmmo = 0;
	int32 SecondaryWeaponID = 0;
	int32 SecondaryCurrentAmmo = 0;
	int32 SecondaryTotalAmmo = 0;
	const TArray<FInventoryListItem>& InventoryData = InventoryComponent->GetItemsRef();

	// 각각 무기 정보 가져오기 ( 현재 총알, 무기 ID, Total 총알)
	if (APubgCharacter* OwnerCharacter = Cast<APubgCharacter>(GetOwningPlayerPawn()))
	{
		if (UEquipManageComponent* EquipManager = Cast<APubgCharacter>(GetOwningPlayerPawn())->GetEquipManageComponent())
		{
				//Primary 정보 가져오기
			if (AWeaponBase* WeaponBase = EquipManager->GetWeaponInSlot(BaseGameplayTags::Item_Slot_Primary))
			{
				if (ABaseGun* BaseGun = Cast<ABaseGun>(WeaponBase))
				{
					PrimaryWeaponID = BaseGun->GetGunSpec().ItemID;
					PrimaryCurrentAmmo = BaseGun->GetCurrentAmmo();
					PrimaryTotalAmmo = OwnerCharacter->GetInventoryComponent()->GetItemAmount(static_cast<uint8>(BaseGun->GetGunSpec().UsedBullet));
				}
				else
				{
					Debug::Print("InventoryWidget :: PrimarySlot : Failed GetWeaponInSlot");
				}
			}

			//Secondary 정보 가져오기
			if (AWeaponBase* WeaponBase = EquipManager->GetWeaponInSlot(BaseGameplayTags::Item_Slot_Secondary))
			{
				if (ABaseGun* BaseGun = Cast<ABaseGun>(WeaponBase))
				{
					SecondaryWeaponID = BaseGun->GetGunSpec().ItemID;
					SecondaryCurrentAmmo = BaseGun->GetCurrentAmmo();
					SecondaryTotalAmmo = OwnerCharacter->GetInventoryComponent()->GetItemAmount(static_cast<uint8>(BaseGun->GetGunSpec().UsedBullet));
				}
			}
			else
			{
				Debug::Print("InventoryWidget :: SecondarySlot : Failed GetWeaponInSlot");
			}
		}
	}
	
	if (UWeaponSlot* NewWeaponSlot = CreateWidget<UWeaponSlot>(this,WeaponSlotClass))
	{
		if (NewWeaponSlot)
		{
			if (PrimaryWeaponID != 0)
			{
				NewWeaponSlot->SetupWeaponSlot(PrimaryWeaponID,PrimaryCurrentAmmo, PrimaryTotalAmmo,BaseGameplayTags::Item_Slot_Primary);
			}
			else
			{
				NewWeaponSlot->DisableWidget();
			}
			
			InventoryCharacter->SetPrimaryWeapon(PrimaryWeaponID);
			NewWeaponSlot->SetSlotTag(BaseGameplayTags::Item_Slot_Primary);
			VB_Primary->AddChildToVerticalBox(NewWeaponSlot);
		}
	}

	if (UWeaponSlot* NewWeaponSlot = CreateWidget<UWeaponSlot>(this,WeaponSlotClass))
	{
		if (NewWeaponSlot)
		{
			if (SecondaryWeaponID != 0)
			{
				NewWeaponSlot->SetupWeaponSlot(SecondaryWeaponID,SecondaryCurrentAmmo, SecondaryTotalAmmo,BaseGameplayTags::Item_Slot_Secondary);
			}
			else
			{
				NewWeaponSlot->DisableWidget();
			}

			InventoryCharacter->SetSecondaryWeapon(SecondaryWeaponID);
			NewWeaponSlot->SetSlotTag(BaseGameplayTags::Item_Slot_Secondary);
			VB_Secondary->AddChildToVerticalBox(NewWeaponSlot);
		}
	}
}

void UInventoryMainWidget::UpdateGear()
{
	WBP_Gear->SetGear();
}

void UInventoryMainWidget::ActiveTooltipBox()
{
	if (!bActiveTooltipBox)
	{
		bActiveTooltipBox = true;
		WBP_ItemTooltip->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UInventoryMainWidget::DeActiveTooltipBox()
{
	if (bActiveTooltipBox)
	{
		bActiveTooltipBox = false;
		WBP_ItemTooltip->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInventoryMainWidget::UpdateWeaponParts()
{
	APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter)
	{
		Debug::Print("WeaponSlot :: Cast Failed Player");
		return;
	}
	
	UEquipManageComponent* EquipManager = PlayerCharacter->GetEquipManageComponent();
	if (!EquipManager)
	{
		Debug::Print("WeaponSlot :: Cast Failed EquipManager");
		return;
	}

	TArray<FGameplayTag> WeaponSlots =
	{
		BaseGameplayTags::Item_Slot_Primary,
		BaseGameplayTags::Item_Slot_Secondary
	};
	
	for (const FGameplayTag& WeaponSlotTag : WeaponSlots)
	{
		if (AWeaponBase* PrimaryWeapon = EquipManager->GetWeaponInSlot(WeaponSlotTag))
		{
			
			UWeaponSlot* PrimarySlot = nullptr;

			if (WeaponSlotTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Primary))
			{
				PrimarySlot= Cast<UWeaponSlot>(VB_Primary->GetChildAt(0));
			}
			else if (WeaponSlotTag.MatchesTagExact(BaseGameplayTags::Item_Slot_Secondary))
			{
				PrimarySlot= Cast<UWeaponSlot>(VB_Secondary->GetChildAt(0));
			}

			TArray<UItemSlot*> EnableParts = PrimarySlot->GetEnableParts();

			if (EnableParts.Num() > 0)
			{
				for (UItemSlot* EnablePart : EnableParts)
				{
					if (AWeaponPartsBase* TempParts = PrimaryWeapon->GetPartsInSlot(EnablePart->GetSlotTag(), false))
					{
						EnablePart->SetSlotImage(TempParts->GetItemID());
					}
				}
			}	
		}
	}

	DisableHighLightWeaponSlot(BaseGameplayTags::Item_Parts);
}
