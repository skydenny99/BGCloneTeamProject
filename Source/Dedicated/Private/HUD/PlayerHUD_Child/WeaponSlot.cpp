// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/WeaponSlot.h"

#include "BaseDebugHelper.h"
#include "BaseGameplayTags.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Characters/PubgCharacter.h"
#include "Components/BasePickUpComponent.h"
#include "Components/BaseWeaponManageComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "Components/Renewal/InventoryManageComponent.h"
#include "Components/Renewal/PickUpManageComponent.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "HUD/HUDBase.h"
#include "HUD/PlayerHUD_Child/DragItemImage.h"
#include "HUD/PlayerHUD_Child/ItemDragDropOperation.h"
#include "HUD/PlayerHUD_Child/ItemSlot.h"
#include "HUD/PlayerHUD_Child/ItemTooltipWidget.h"
#include "HUD/UI/InventoryMainWidget.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"
#include "Item/BaseGearFieldItem.h"
#include "Item/BaseWeaponFieldItem.h"


UWeaponSlot::UWeaponSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UDragItemImage> DragWidgetAsset(TEXT("/Game/_BP/UI/Inventory/Module/WBP_DragItemImgae.WBP_DragItemImgae_C"));
	if (DragWidgetAsset.Succeeded())
		DragItemWidgetClass = DragWidgetAsset.Class;
}

void UWeaponSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWeaponSlot::SetWeaponImage()
{
	Img_WeaponRender->SetBrushFromTexture(GetWeaponTexture(ItemID));
}


void UWeaponSlot::SetupWeaponSlot(const int32 ItemItemID, const int32 CurrentAmmo, const int32 TotalAmmo, const FGameplayTag& WeaponType)
{
	if(const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(ItemItemID))
	{
		ItemID = ItemItemID;
		WeaponTag = WeaponType;
		SetupSlotNumber( WeaponTag);
		Text_WeaponName->SetText(ItemData->DisplayName);
		Text_CurrentAmmo->SetText(FText::AsNumber(CurrentAmmo));
		Text_TotalAmmo->SetText(FText::AsNumber(TotalAmmo));
		InitWeaponSlot();
		SetWeaponImage();
		EnableWidget();
	}
}

void UWeaponSlot::SetupSlotNumber(const FGameplayTag& WeaponType)
{
	if (WeaponType.MatchesTag(BaseGameplayTags::Item_Slot_Primary))
		Text_SlotNumber->SetText(FText::FromString(TEXT("1")));
	else if (WeaponType.MatchesTag(BaseGameplayTags::Item_Slot_Secondary))
		Text_SlotNumber->SetText(FText::FromString(TEXT("2")));
	else if (WeaponType.MatchesTag(BaseGameplayTags::Item_Slot_SideArm))
		Text_SlotNumber->SetText(FText::FromString(TEXT("3")));
	else if (WeaponType.MatchesTag(BaseGameplayTags::Item_Slot_Melee))
		Text_SlotNumber->SetText(FText::FromString(TEXT("4")));
	else
		Text_SlotNumber->SetText(FText::FromString(TEXT("5")));
}

void UWeaponSlot::DisableWidget()
{
	EnablePartsSlot(false,false,false,false,false,false);
	bIsEnable = false;
	Img_Hovered->SetVisibility(ESlateVisibility::Collapsed);
	Img_Disable->SetVisibility(ESlateVisibility::Collapsed);
	Img_Normal->SetVisibility(ESlateVisibility::HitTestInvisible);
	Img_Pressed->SetVisibility(ESlateVisibility::Collapsed);
	Img_Pick->SetVisibility(ESlateVisibility::Collapsed);
	Img_AmmoBG->SetVisibility(ESlateVisibility::Collapsed);
	Img_WeaponRender->SetVisibility(ESlateVisibility::Collapsed);
	Img_SlotFrame->SetVisibility(ESlateVisibility::Collapsed);
	Img_Ammo->SetVisibility(ESlateVisibility::Collapsed);
	Text_WeaponName->SetVisibility(ESlateVisibility::Collapsed);
	Text_SlotNumber->SetVisibility(ESlateVisibility::Collapsed);
	Text_CurrentAmmo->SetVisibility(ESlateVisibility::Collapsed);
	Text_TotalAmmo->SetVisibility(ESlateVisibility::Collapsed);
	Text_AmmoType->SetVisibility(ESlateVisibility::Collapsed);
}

void UWeaponSlot::EnableWidget()
{
	bIsEnable = true;
	Img_Hovered->SetVisibility(ESlateVisibility::Collapsed);
	Img_Disable->SetVisibility(ESlateVisibility::Collapsed);
	Img_Normal->SetVisibility(ESlateVisibility::HitTestInvisible);
	Img_Pressed->SetVisibility(ESlateVisibility::Collapsed);
	Img_Pick->SetVisibility(ESlateVisibility::Collapsed);
	Img_WeaponRender->SetVisibility(ESlateVisibility::HitTestInvisible);
	Img_SlotFrame->SetVisibility(ESlateVisibility::HitTestInvisible);
	Img_Ammo->SetVisibility(ESlateVisibility::HitTestInvisible);
	Img_AmmoBG->SetVisibility(ESlateVisibility::HitTestInvisible);
	Text_WeaponName->SetVisibility(ESlateVisibility::HitTestInvisible);
	Text_SlotNumber->SetVisibility(ESlateVisibility::HitTestInvisible);
	Text_CurrentAmmo->SetVisibility(ESlateVisibility::HitTestInvisible);
	Text_TotalAmmo->SetVisibility(ESlateVisibility::HitTestInvisible);
	Text_AmmoType->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (Btn_WeaponSlot)
	{
		Btn_WeaponSlot->OnHovered.AddDynamic(this, &ThisClass::OnSlotHovered);
		Btn_WeaponSlot->OnUnhovered.AddDynamic(this, &ThisClass::OnSlotUnHovered);
	}
}

FReply UWeaponSlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		// 만약 생성만하고 아이템이 설정 안되어있는걸 선택할 경우.
		if (ItemID <= 0) return FReply::Unhandled();

		TMap<UItemSlot*, FString> SlotMap = {
			{Slot_Scope, "Slot_Scope :: ScopeClicked!!"},
			{Slot_Magazine, "Slot_Magazine :: MagazineClicked!!"},
			{Slot_Stock, "Slot_Stock :: StockClicked!!"},
			{Slot_Muzzle, "Slot_Muzzle :: MuzzleClicked!!"},
			{Slot_SideScope, "Slot_SideScope :: SideScopeClicked!!"},
			{Slot_Grip, "Slot_Grip :: GripClicked!!"}
		};
		
		
		if (Btn_WeaponSlot && Btn_WeaponSlot->IsHovered())
		{
			Debug::Print("Weapon Slot :: Clicked!");
			bIsSlotClick = false;
			return  UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent,this,EKeys::LeftMouseButton).NativeReply;
		}

		for (const TPair<UItemSlot*, FString>& TempSlot : SlotMap)
		{
			if (TempSlot.Key  && TempSlot.Key->IsHovered())
			{
				Debug::Print(TempSlot.Value);
				bIsSlotClick = true;
				SlotItemID = TempSlot.Key->GetItemID();
				SlotItemTag = TempSlot.Key->GetSlotTag();
				return  UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent,this,EKeys::LeftMouseButton).NativeReply; 
			}
		}
	}

	
	
	return FReply::Unhandled();
}

void UWeaponSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UDragItemImage* DragWidget = nullptr;
	if (DragItemWidgetClass)
	{
		DragWidget = CreateWidget<UDragItemImage>(GetWorld(),DragItemWidgetClass);
		if (bIsSlotClick && SlotItemID  != -1)
			DragWidget->InitSlot(SlotItemID);
		else
			DragWidget->InitSlot(ItemID);
		Debug::Print("ItemId : ", ItemID);
	}
		
	UItemDragDropOperation* DragOperation = Cast<UItemDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UItemDragDropOperation::StaticClass()));

	if (DragOperation && IsValid(DragWidget))
	{
		DragOperation->DefaultDragVisual = DragWidget;

		if (bIsSlotClick && SlotItemID != -1)
			DragOperation->Initialize(SlotItemID,SlotItemTag,SlotTag,true);
		else
			DragOperation->Initialize(ItemID,WeaponTag,true);
	}

	if (APubgCharacter* PlayerCharacter =  Cast<APubgCharacter>(GetOwningPlayerPawn()))
	{
		PlayerCharacter->GetHUDBase()->GetInventoryMainWidget()->EnableHighLightWeaponSlot(SlotItemTag);
	}
	OutOperation = DragOperation;
}

void UWeaponSlot::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	if(const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(ItemID))
	{
		if (ItemData->ItemType == EItemType::Mainweapon)
		{
			SlotPressed();
		}
	}
}

void UWeaponSlot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	if(const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(ItemID))
	{
		if (ItemData->ItemType == EItemType::Mainweapon)
		{
			SlotPicked();
		}
	}
}

bool UWeaponSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (UItemDragDropOperation* DragDropOperation = Cast<UItemDragDropOperation>(InOperation))
	{
		OnSlotUnHovered();
		APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(GetOwningPlayerPawn());
		if (!PlayerCharacter)
		{
			Debug::Print("Cast Failed PubgCharacter");
			return false;
		}

		// 전달받은 객체의 태그 설정   // 주변아이템이 아니면 GetFieldItem을 안가지고 있음.
		FGameplayTag TempWeaponTag = (DragDropOperation->GetFieldItem() != nullptr)
										  ? DragDropOperation->GetFieldItem()->GetItemType()
										  : DragDropOperation->GetWeaponTag();

		Debug::Print(FString::Printf(TEXT("WeaponSlot :: Tag %s"),*TempWeaponTag.ToString()));
		// 주변 아이템에서 가져온건지 검사
		if (ABaseFieldItem* BaseFiledItem = Cast<ABaseFieldItem>(DragDropOperation->GetFieldItem()))
		{
			// 무기 검사
			if (ABaseWeaponFieldItem* TempWeaponFieldItem = Cast<ABaseWeaponFieldItem>(BaseFiledItem))
			{
				if (TempWeaponFieldItem->GetEquippableTags().HasAny(SlotTag.GetSingleTagContainer()))
				{
					Debug::Print(FString::Printf(TEXT("UWeaponSlot :: Equip Weapon Tag %s"), *TempWeaponTag.ToString()), FColor::Green);

					ABaseWeaponFieldItem* WeaponItem = Cast<ABaseWeaponFieldItem>(DragDropOperation->GetFieldItem());
					// 무기의 장착가능한 테그에 자신의 테그가 포함되어 있는지 확인.
					const FGameplayTagContainer& Container = WeaponItem->GetEquippableTags();
					if (Container.HasAny(SlotTag.GetSingleTagContainer()))
					{
						PlayerCharacter->GetPickUpManageComponent()->Server_PickUpWithSlot(WeaponItem, SlotTag);
					}

					return true;
				}
			}
			// 파츠 검사
			else if (BaseFiledItem->GetItemType().MatchesTag(BaseGameplayTags::Item_Parts))
			{
				Debug::Print(FString::Printf(TEXT("UWeaponSlot :: Equip Parts Tag %s"), *TempWeaponTag.ToString()), FColor::Green);
				// TODO:: 파츠 장착 로직 진행
				PlayerCharacter->GetPickUpManageComponent()->Server_PickUpWithSlot(BaseFiledItem,SlotTag);
				return true;
			}
		}

		static const FGameplayTagContainer ValidSlotTags = CreateValidSlotTags();
		
		// 무기를 스왑할려고 하는가?
		if (TempWeaponTag.MatchesAny(ValidSlotTags) && DragDropOperation->GetFieldItem() == nullptr)
		{
			Debug::Print(FString::Printf(TEXT("UWeaponSlot :: Weapon Swap Tag from %s, to %s"), *TempWeaponTag.ToString(), *SlotTag.ToString()), FColor::Green);
			PlayerCharacter->GetEquipManageComponent()->Server_SwapWeapon(TempWeaponTag, SlotTag);
			/*PlayerCharacter->GetEquipManageComponent()->SwapWeapon(BaseGameplayTags::Item_Slot_Secondary, BaseGameplayTags::Item_Slot_Primary);
			Debug::Print("Try WeaponSwap");*/

			return true;
		}

		// 파츠아이템을 인벤토리에서 가져왔는가?
		if (DragDropOperation->GetFieldItem() == nullptr)
		{
			int32 TempItemID = DragDropOperation->GetItemID();

			if(const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(TempItemID))
			{
				if (ItemData->ItemType == EItemType::Attachments)
				{
					PlayerCharacter->GetInventoryComponent()->Server_EquipParts(DragDropOperation->GetItemID(), SlotTag);
					Debug::Print("Equip Parts From Inventory");
					return true;
				}
					
			}
			
		}


		FString TempString = FString::Printf(TEXT("UWeaponSlot :: InValid Item Tag %s"), *TempWeaponTag.ToString());
		Debug::Print(TempString, FColor::Red);
		PlayerCharacter->GetHUDBase()->InventoryMainWidget->DisableHighLightWeaponSlot(SlotItemTag);
		return false;
	}
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

UTexture2D* UWeaponSlot::GetWeaponTexture(const int32 _ItemID)
{
	return WeaponArray.Find(_ItemID) ? *WeaponArray.Find(_ItemID) : *WeaponArray.Find(0);
}


void UWeaponSlot::EnablePartsSlot(const bool bMuzzle, const bool bMagazine, const bool bStock, const bool bGrip,
                                  const bool bScope, const bool bSideScope)
{
	if (bMuzzle)
	{
		Slot_Muzzle->SetVisibility(ESlateVisibility::Visible);
		Slot_Muzzle->SetParts(true);
		EnableParts.Add(Slot_Muzzle);
	}
	if (bMagazine)
	{
		Slot_Magazine->SetVisibility(ESlateVisibility::Visible);
		Slot_Magazine->SetParts(true);
		EnableParts.Add(Slot_Magazine);
	}
	if (bStock)
	{
		Slot_Stock->SetVisibility(ESlateVisibility::Visible);
		Slot_Stock->SetParts(true);
		EnableParts.Add(Slot_Stock);
	}
	if (bGrip)
	{
		Slot_Grip->SetVisibility(ESlateVisibility::Visible);
		Slot_Grip->SetParts(true);
		EnableParts.Add(Slot_Grip);
	}
	if (bScope)
	{
		Slot_Scope->SetVisibility(ESlateVisibility::Visible);
		Slot_Scope->SetParts(true);
		EnableParts.Add(Slot_Scope);
	}
	if (bSideScope)
	{
		Slot_SideScope->SetVisibility(ESlateVisibility::Visible);
		Slot_SideScope->SetParts(true);
		EnableParts.Add(Slot_SideScope);
	}
}

void UWeaponSlot::InitWeaponSlot()
{
	if (ItemID == 1) //AKM
	{
		EnablePartsSlot(true,true,false,false,true,true);
		SetupAmmoType(EBulletType::B_7_62mm);
		return;
	}

	if (ItemID == 2) //  GROZA
	{
		EnablePartsSlot(true,true,false,false,true,false);
		SetupAmmoType(EBulletType::B_7_62mm);
		return;
	}

	if (ItemID == 3) // m16
	{
		EnablePartsSlot(true,true,true,false,true,true);
		SetupAmmoType(EBulletType::B_5_56mm);
		return;
	}
	
	if (ItemID == 4) // AUG
	{
		EnablePartsSlot(true, true, false, true, true, true);
		SetupAmmoType(EBulletType::B_5_56mm);
		return;
	}

	if (ItemID == 5) // Saiga
	{
		EnablePartsSlot(true, true, false, false, true, true);
		SetupAmmoType(EBulletType::B_12gauge);
		return;
	}

	if (ItemID == 6) // m1897
	{
		EnablePartsSlot(true, false, true, false, false, false);
		SetupAmmoType(EBulletType::B_12gauge);
		return;
	}

	if (ItemID == 7) // m24
	{
		EnablePartsSlot(true, true, true, false, true, true);
		SetupAmmoType(EBulletType::B_7_62mm);
		return;
	}
	if (ItemID == 8) // vss
	{
		EnablePartsSlot(false, true, true, false, false, true);
		SetupAmmoType(EBulletType::B_9mm);
		return;
	}
	if (ItemID == 9) //uzi
	{
		EnablePartsSlot(true, true, true, false, true, false);
		SetupAmmoType(EBulletType::B_9mm);
		return;
	}
	if (ItemID == 10) // ump
	{
		EnablePartsSlot(true, true, false, true, true, true);
		SetupAmmoType(EBulletType::B_9mm);
		return;
	}
	if (ItemID == 50) //g18
	{
		EnablePartsSlot(true, true, false, true, true, false);
		SetupAmmoType(EBulletType::B_9mm);
		return;
	}
	if (ItemID == 51) //sawed-off
	{
		EnablePartsSlot(true, false, false, false, false, false);
		SetupAmmoType(EBulletType::B_12gauge);
		return;
	}
}

void UWeaponSlot::SetupAmmoType(EBulletType AmmoType)
{
	switch (AmmoType)
	{
	case EBulletType::B_5_56mm:
		Img_Ammo->SetBrushFromTexture(AmmoArray[0]);
		Text_AmmoType->SetText(FText::FromString(TEXT("5.56mm")));
		break;
	case EBulletType::B_7_62mm:
		Img_Ammo->SetBrushFromTexture(AmmoArray[1]);
		Text_AmmoType->SetText(FText::FromString(TEXT("7.62mm")));
		break;
	case EBulletType::B_9mm:
		Img_Ammo->SetBrushFromTexture(AmmoArray[2]);
		Text_AmmoType->SetText(FText::FromString(TEXT("9mm")));
		break;
	case EBulletType::B_12gauge:
		Img_Ammo->SetBrushFromTexture(AmmoArray[3]);
		Text_AmmoType->SetText(FText::FromString(TEXT("12 게이지")));
		break;
	}
}

void UWeaponSlot::OnSlotHovered()
{
	Img_Hovered->SetVisibility(ESlateVisibility::HitTestInvisible);
	Img_Disable->SetVisibility(ESlateVisibility::Collapsed);
	Img_Normal->SetVisibility(ESlateVisibility::Collapsed);
	Img_Pressed->SetVisibility(ESlateVisibility::Collapsed);
	Img_Pick->SetVisibility(ESlateVisibility::Collapsed);

	if (APubgCharacter* PlayerController = Cast<APubgCharacter>(GetOwningPlayerPawn()))
	{
		PlayerController->GetHUDBase()->GetInventoryMainWidget()->GetTooltipWidget()->SetupItemTooltip(ItemID);
		PlayerController->GetHUDBase()->GetInventoryMainWidget()->ActiveTooltipBox();
	}
}


void UWeaponSlot::OnSlotUnHovered()
{
	Img_Hovered->SetVisibility(ESlateVisibility::Collapsed);
	Img_Disable->SetVisibility(ESlateVisibility::Collapsed);
	Img_Normal->SetVisibility(ESlateVisibility::HitTestInvisible);
	Img_Pressed->SetVisibility(ESlateVisibility::Collapsed);
	Img_Pick->SetVisibility(ESlateVisibility::Collapsed);
	
	if (APubgCharacter* PlayerController = Cast<APubgCharacter>(GetOwningPlayerPawn()))
	{
		PlayerController->GetHUDBase()->GetInventoryMainWidget()->DeActiveTooltipBox();
	}
}

void UWeaponSlot::SlotPicked()
{
	Img_Hovered->SetVisibility(ESlateVisibility::Collapsed);
	Img_Disable->SetVisibility(ESlateVisibility::Collapsed);
	Img_Normal->SetVisibility(ESlateVisibility::Collapsed);
	Img_Pressed->SetVisibility(ESlateVisibility::Collapsed);
	Img_Pick->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UWeaponSlot::SlotPressed()
{
	Img_Hovered->SetVisibility(ESlateVisibility::Collapsed);
	Img_Disable->SetVisibility(ESlateVisibility::Collapsed);
	Img_Normal->SetVisibility(ESlateVisibility::Collapsed);
	Img_Pressed->SetVisibility(ESlateVisibility::HitTestInvisible);
	Img_Pick->SetVisibility(ESlateVisibility::Collapsed);
}

FGameplayTagContainer UWeaponSlot::CreateValidSlotTags()
{
	FGameplayTagContainer ValidSlotTags;
	ValidSlotTags.AddTag(BaseGameplayTags::Item_Slot_Primary);
	ValidSlotTags.AddTag(BaseGameplayTags::Item_Slot_Secondary);
	return ValidSlotTags;
}

void UWeaponSlot::ActiveHighlight(const FGameplayTag _PartsTag)
{
	TArray<UItemSlot*> PartsSlots =
	{
		Slot_Scope, Slot_Grip, Slot_Magazine, Slot_Muzzle, Slot_Stock, Slot_SideScope
	};

	for (UItemSlot* PartsSlot : PartsSlots)
	{
		if (_PartsTag.MatchesTagExact(PartsSlot->GetSlotTag()))
		{
			PartsSlot->OnSlotPressed();
			Debug::Print("Selected Tag : " + _PartsTag.ToString());
			return;
		}
	}
}

void UWeaponSlot::DeActiveHighlight()
{
	TArray<UItemSlot*> PartsSlots =
	{
		Slot_Scope, Slot_Grip, Slot_Magazine, Slot_Muzzle, Slot_Stock, Slot_SideScope
	};

	for (UItemSlot* PartsSlot : PartsSlots)
	{
		PartsSlot->OnSlotNormal();
	}
}
