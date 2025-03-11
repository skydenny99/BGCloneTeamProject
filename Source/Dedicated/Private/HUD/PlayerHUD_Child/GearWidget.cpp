// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/GearWidget.h"

#include "BaseDebugHelper.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Characters/PubgCharacter.h"
#include "Characters/PubgInventoryCharacter.h"
#include "Components/Image.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "Components/Renewal/PickUpManageComponent.h"
#include "HUD/PlayerHUD_Child/ItemSlot.h"
#include "Engine/TextureRenderTarget2D.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "HUD/HUDBase.h"
#include "HUD/PlayerHUD_Child/DragItemImage.h"
#include "HUD/PlayerHUD_Child/ItemDragDropOperation.h"
#include "HUD/UI/InventoryMainWidget.h"
#include "Subsystem/PubgDataTableManageSubsystem.h"


class UPubgDataTableManager;

void UGearWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UItemSlot*> ItemSlots = {
		Slot_Helmet, Slot_Vest, Slot_Backpack, Slot_Ghillie, CostumeSlot_Head,
		CostumeSlot_Glass, CostumeSlot_Mask, CostumeSlot_Top, CostumeSlot_Outerwear, CostumeSlot_Glove,
		CostumeSlot_Bottom, CostumeSlot_Boots
	};

	for (UItemSlot* ItemSlot : ItemSlots)
	{
		if (ItemSlot)
			ItemSlot->SetParts(false);
	}
}

UGearWidget::UGearWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UDragItemImage> DragWidgetAsset(TEXT("/Game/_BP/UI/Inventory/Module/WBP_DragItemImgae.WBP_DragItemImgae_C"));
	if (DragWidgetAsset.Succeeded())
		DragItemWidgetClass = DragWidgetAsset.Class;
}

void UGearWidget::SetGear()
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

	UEquipManageComponent* EquipComponent = PlayerCharacter->GetEquipManageComponent();
	if (!EquipComponent)
	{
		Debug::Print("UpdateWeapon :: EquipComponent is nullptr");
		return;
	}
	
	TArray<UItemSlot*> ItemSlots = { Slot_Helmet, Slot_Vest, Slot_Backpack};
	for (UItemSlot* ItemSlot : ItemSlots)
	{
		if (ABaseGear* EquipGear =  EquipComponent->GetGearInSlot(ItemSlot->GetSlotTag()))
		{
			if(const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(EquipGear->GetGearItemID()))
			{
				ItemSlot->SetSlotImage(ItemData->ItemIcon,EquipGear->GetGearItemID());
				//PlayerCharacter->GetPubgInventoryCharacter()->UpdateGear(EquipGear);
				Debug::Print("Gear Item ID : ",EquipGear->GetGearItemID());
			}
		}
		else
		{
			ItemSlot->InitSlotImage();
		}
	}
}

void UGearWidget::SetRenderTarget(UTextureRenderTarget2D* RenderTarget)
{
	if (!RenderTarget)
	{
		Debug::Print("UGearWidget :: Invalid Render Target",FColor::Red);
		return;
	}

	// RenderTarget을 설정하기 위해 현재 설정된 MaterialInstance 정보 가져오기.
	UObject* ResourceObject = Img_Character->GetBrush().GetResourceObject();
	UMaterialInterface* Material = Cast<UMaterialInterface>(ResourceObject);
	
	if (!DynamicMaterialInstance)
	{
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
		Img_Character->SetBrushFromMaterial(DynamicMaterialInstance);
		Debug::Print("SetRenderTarget",FColor::Green);
	}

	DynamicMaterialInstance->SetTextureParameterValue(TEXT("Resource"), RenderTarget);

	//Img_Character->InvalidateLayoutAndVolatility();
}

void UGearWidget::ActiveHighLight(const FGameplayTag _GearTag)
{
	if (_GearTag.MatchesTagExact(BaseGameplayTags::Item_Gear_Armor))
	{
		Slot_Vest->OnSlotPressed();
		return;
	}

	if (_GearTag.MatchesTagExact(BaseGameplayTags::Item_Gear_Backpack))
	{
		Slot_Backpack->OnSlotPressed();
		return;
	}

	if (_GearTag.MatchesTagExact(BaseGameplayTags::Item_Gear_Helmet))
	{
		Slot_Helmet->OnSlotPressed();
		return;
	}
}

void UGearWidget::DeActiveHighLight()
{
	TArray<UItemSlot*> ItemSlots = { Slot_Helmet, Slot_Vest, Slot_Backpack};

	for (UItemSlot* ItemSlot : ItemSlots)
	{
		ItemSlot->OnSlotNormal();
	}
}

bool UGearWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                               UDragDropOperation* InOperation)
{
	if (UItemDragDropOperation* DragDropOperation = Cast<UItemDragDropOperation>(InOperation))
	{
		APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(GetOwningPlayerPawn());
		if (!PlayerCharacter)
		{
			Debug::Print("Cast Failed PubgCharacter");
			return false;
		}

		PlayerCharacter->GetPickUpManageComponent()->PickUp(DragDropOperation->GetFieldItem());

		PlayerCharacter->GetHUDBase()->GetInventoryMainWidget()->DisableHighLightWeaponSlot(BaseGameplayTags::Item_Gear);
	}
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

FReply UGearWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (Slot_Vest && Slot_Vest->IsHovered())
		{
			Debug::Print("Vest Slot :: Clicked!");
			HoveredSlotItemID = Slot_Vest->GetItemID();
			HoveredSlotTag = Slot_Vest->GetSlotTag();
		}

		if (Slot_Backpack && Slot_Backpack->IsHovered())
		{
			Debug::Print("Backpack Slot :: Clicked!");
			HoveredSlotItemID = Slot_Backpack->GetItemID();
			HoveredSlotTag = Slot_Backpack->GetSlotTag();
		}

		if (Slot_Helmet && Slot_Helmet->IsHovered())
		{
			Debug::Print("Helmet Slot :: Clicked!");
			HoveredSlotItemID = Slot_Helmet->GetItemID();
			HoveredSlotTag = Slot_Helmet->GetSlotTag();
		}
		if (HoveredSlotItemID <= 0) return FReply::Unhandled();
		
		return  UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent,this,EKeys::LeftMouseButton).NativeReply;
	}

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (Slot_Vest && Slot_Vest->IsHovered())
		{
			Debug::Print("Vest Slot :: RightClicked!");
			HoveredSlotTag = Slot_Vest->GetSlotTag();
		}

		if (Slot_Backpack && Slot_Backpack->IsHovered())
		{
			Debug::Print("Backpack Slot :: RightClicked!");
			HoveredSlotTag = Slot_Backpack->GetSlotTag();
		}

		if (Slot_Helmet && Slot_Helmet->IsHovered())
		{
			Debug::Print("Helmet Slot :: RightClicked!");
			HoveredSlotTag = Slot_Helmet->GetSlotTag();
		}

		if (HoveredSlotTag.IsValid())
		{
			if (APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(GetOwningPlayerPawn()))
			{
				PlayerCharacter->GetPickUpManageComponent()->Server_DiscardGear(HoveredSlotTag);
			}
		}
		
		
	}
	
	
	return FReply::Unhandled();
}

void UGearWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UDragItemImage* DragWidget = nullptr;
	if (DragItemWidgetClass)
	{
		DragWidget = CreateWidget<UDragItemImage>(GetWorld(),DragItemWidgetClass);
		DragWidget->InitSlot(HoveredSlotItemID);
		Debug::Print(FString::Printf(TEXT("SlotID : %d,    Slot Tag : %s"),HoveredSlotItemID, *HoveredSlotTag.ToString()));
	}
		
	UItemDragDropOperation* DragOperation = Cast<UItemDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UItemDragDropOperation::StaticClass()));

	if (DragOperation && IsValid(DragWidget))
	{
		DragOperation->DefaultDragVisual = DragWidget;
		DragOperation->Initialize(HoveredSlotItemID,HoveredSlotTag,true);
	}
	
	OutOperation = DragOperation;
}
