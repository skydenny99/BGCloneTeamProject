// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HUDBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/PlayerHUD_Child/CrosshairWidget.h"
#include "HUD/PlayerHUD/PlayerHUD.h"
#include "Characters/PubgCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/BaseWeaponManageComponent.h"
#include "HUD/PlayerHUD_Child/SummaryWidget.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "Components/Renewal/InventoryManageComponent.h"
#include "HUD/UI/InventoryMainWidget.h"
#include "Weapon/BaseGun.h"

class UInventoryMainWidget;

void AHUDBase::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 HUD 생성
	if (PlayerHUDWidgetClass)
	{
		PlayerHUD = CreateWidget<UPlayerHUD>(GetWorld(), PlayerHUDWidgetClass);

		if (PlayerHUD)
		{
			PlayerHUD->AddToViewport();
		}
	}

	// 크로스헤어 위젯 생성
	if (CrosshairWidgetClass)
	{
		CrosshairWidget = CreateWidget<UCrosshairWidget>(GetWorld(),CrosshairWidgetClass);
		
		if (CrosshairWidget)
		{
			CrosshairWidget->AddToViewport();
			
			// 초기 CrosshairSpread 값
			CrosshairSpread=0.0f;
		}
	}
	
	// Inventory Widget 생성
	if (InventoryWidgetClass) 
	{
		InventoryMainWidget = CreateWidget<UInventoryMainWidget>(GetWorld(), InventoryWidgetClass);
		if (InventoryMainWidget)
		{
			InventoryMainWidget->AddToViewport();
			InventoryMainWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	//Summary Widget 생성
	if (SummaryWidgetClass)
	{
		SummaryWidget = CreateWidget<USummaryWidget>(GetWorld(), SummaryWidgetClass);
		if (SummaryWidget)
		{
			SummaryWidget->AddToViewport();
			SummaryWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	InitDeligate();
}

void AHUDBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCrosshairSpread();
}

void AHUDBase::SetHUDVisibility(bool bVisible)
{
	if (PlayerHUD)
	{
		PlayerHUD->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (CrosshairWidget)
	{
		CrosshairWidget->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void AHUDBase::UpdateCrosshairSpread()
{
	APubgCharacter* Player = Cast<APubgCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!Player)
	{
		CrosshairSpread=0.0f;
		return;
	}

	// 플레이어 속도 계산
	FVector Velocity=Player->GetVelocity();
	float Speed=Velocity.Size();

	// MapRangeClamped 구현(속도를 Crosshair Spread로 매핑)
	const float MinSpeed=0.0f;
	const float MaxSpeed=450.0f;
	const float MinSpread=15.0f;
	const float MaxSpread=80.0f;

	CrosshairSpread=FMath::GetMappedRangeValueClamped(
		FVector2D(MinSpeed,MaxSpeed),
		FVector2D(MinSpread,MaxSpread),
		Speed	
	);

	// Crosshair 위젯에 Spread 값을 전달
	if (CrosshairWidget)
	{
		CrosshairWidget->CrosshairSpread=CrosshairSpread;
	}
}

float AHUDBase::GetPlayerSpeed() const
{
	APubgCharacter* Player=Cast<APubgCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!Player)
	{
		return 0.0f;
	}

	// 속도 계산(Velocity 크기)
	return Player->GetVelocity().Size();
}

void AHUDBase::ToggleInventory()
{
		if (!InventoryMainWidget) return;

		
	
		if (bIsInventoryOpen)
		{
			InventoryMainWidget->SetVisibility(ESlateVisibility::Hidden);
			GetOwningPlayerController()->bShowMouseCursor = false;  // 마우스 커서 숨김
			GetOwningPlayerController()->SetInputMode(FInputModeGameOnly()); // 게임 모드로 전환
			SetHUDVisibility(bIsInventoryOpen);
		}
		else
		{
			InventoryMainWidget->DeActiveTooltipBox();
			InventoryMainWidget->UpdateInventory(); // 인벤토리 데이터 갱신
			InventoryMainWidget->UpdateAroundItem();
			InventoryMainWidget->UpdateWeapon();
			InventoryMainWidget->UpdateWeaponParts();
			InventoryMainWidget->SetVisibility(ESlateVisibility::Visible);
			SetHUDVisibility(bIsInventoryOpen);
			GetOwningPlayerController()->SetInputMode(FInputModeGameAndUI()); // GameAndUI 모드로 전환
			//GetOwningPlayerController()->SetInputMode(FInputModeUIOnly()); // UI 모드로 전환

			// Get Screen Size
			int32 ViewportSizeX, ViewportSizeY;
			GetOwningPlayerController()->GetViewportSize(ViewportSizeX, ViewportSizeY);
			GetOwningPlayerController()->SetMouseLocation(ViewportSizeX /2, ViewportSizeY /2);
			GetOwningPlayerController()->bShowMouseCursor = true;  // 마우스 커서 표시
		}

		bIsInventoryOpen = !bIsInventoryOpen;
	
	
}

void AHUDBase::ToogleCrosshairWidget(const bool _Active)
{
	if (_Active)
	{
		CrosshairWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		CrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AHUDBase::InitDeligate()
{
	// 델리게이트 바인딩
	APubgCharacter* PubgCharacter = Cast<APubgCharacter>(GetOwningPlayerController()->GetPawn());
	if (PubgCharacter)
	{
		if (UEquipManageComponent* EquipManager = PubgCharacter->GetEquipManageComponent())
		{
			EquipManager->OnCurrentWeaponUpdatedDelegate.BindUObject(this, &AHUDBase::OnUpdateCurrentWeapon);
		}
	}
}

void AHUDBase::OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMode, uint8 PreviousCustomMode)
{
	// MovementMode가 변경될 때 Crosshair Spread 업데이트
	UpdateCrosshairSpread();
}

void AHUDBase::OnUpdateCurrentWeapon(AWeaponBase* InCurrentWeapon)
{
		if (ABaseGun* CurrentGun = Cast<ABaseGun>(InCurrentWeapon))
		{
			if (IsValid(CurrentGun))
			{
				CurrentGun->OnUpdateWeaponInfoDelegate.AddUObject(this, &AHUDBase::OnUpdateCurrentGunInfo);
				CurrentGun->OnUpdateCurrentAmmoDelegate.AddUObject(this, &AHUDBase::OnUpdateCurrentGunAmmo);
				OnUpdateCurrentGunInfo(CurrentGun);
			}
		}
}
void AHUDBase::OnUpdateCurrentGunInfo(AWeaponBase* InCurrentWeapon)
{
	if (ABaseGun* CurrentGun = Cast<ABaseGun>(InCurrentWeapon))
	{
		PlayerHUD->UpdateFireMode(CurrentGun->GetCurrentFireMode());
		PlayerHUD->UpdateCurrentDecreaseAmmo(CurrentGun->GetCurrentAmmo());
		if (APubgCharacter* PubgCharacter = Cast<APubgCharacter>(GetOwningPlayerController()->GetPawn()))
		{
			PlayerHUD->UpdatePossessedAmmo(PubgCharacter->GetInventoryComponent()->GetItemAmount(static_cast<uint8>(CurrentGun->GetGunSpec().UsedBullet)));	
		}
	}
}

void AHUDBase::OnUpdateCurrentGunAmmo(int32 InCurrentAmmo)
{
	PlayerHUD->UpdateCurrentDecreaseAmmo(InCurrentAmmo);
}

void AHUDBase::OnUpdatePlayerHUDAmmoSlate(FGameplayTag FireMode, int32 CurrentAmmo, int32 TotalAmmo)
{
	PlayerHUD->UpdateFireMode(FireMode);
	PlayerHUD->UpdateCurrentDecreaseAmmo(CurrentAmmo);
	PlayerHUD->UpdatePossessedAmmo(TotalAmmo);	
}



void AHUDBase::OnWeaponEquipped()
{
	
}

void AHUDBase::OnWeaponUnequipped()
{
	PlayerHUD->UnEquipWeapon();
}

void AHUDBase::OnUpdateWeaponFireMode(FGameplayTag FireMode)
{
}

void AHUDBase::OnUpdateMaxAmmoCount(int32 Val)
{
	PlayerHUD->UpdatePossessedAmmo(Val);
}

