// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "HUDBase.generated.h"

class USummaryWidget;
class AWeaponBase;
struct FGameplayTag;
enum class EWeaponFireMode : uint8;
class UInventoryMainWidget;
class UPlayerHUD;
class UCrosshairWidget;
/**
 * 
 */
UCLASS()
class DEDICATED_API AHUDBase : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> SummaryWidgetClass;
	
	UPROPERTY()
	UPlayerHUD*PlayerHUD;
	UPROPERTY()
	UCrosshairWidget* CrosshairWidget;
	UPROPERTY()
	USummaryWidget* SummaryWidget;
	
	

public:
	// Crosshair Spread 값 
	float CrosshairSpread = 0;

public:
	virtual void BeginPlay() override;

	// TODO 플레이어 움직임을 감지하는 함수가 있으면 Tick 삭제
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Blueprintable)
	void SetHUDVisibility(bool bVisible);
	
private:
	// Crosshair Spread를 계산하는 함수
	void UpdateCrosshairSpread();

	// MovementMode 변경 이벤트 처리 함수
	void OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMode, uint8 PreviousCustomMode);

	// 현재 플레이어의 속도를 가져오는 함수
	float GetPlayerSpeed() const;
public:
	/** 인벤토리 UI 위젯 */
	UPROPERTY()
	UInventoryMainWidget* InventoryMainWidget;

	/** 인벤토리 UI 클래스 (Blueprint에서 설정) */
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UInventoryMainWidget> InventoryWidgetClass;

	/** 인벤토리 UI가 열려있는지 여부 */
	bool bIsInventoryOpen = false;
	
	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

	void ToogleCrosshairWidget(const bool _Active);

	// 델리게이트 바인드함수
	UFUNCTION()
	void InitDeligate();
	
public:
	FORCEINLINE UPlayerHUD* GetPlayerHUD() { return PlayerHUD; }
	FORCEINLINE UInventoryMainWidget* GetInventoryMainWidget() { return InventoryMainWidget; }
	FORCEINLINE bool GetInventoryOpen() { return bIsInventoryOpen; }
	FORCEINLINE UCrosshairWidget* GetCrosshairWidget() { return CrosshairWidget; }
	FORCEINLINE USummaryWidget* GetSummaryWidget() { return SummaryWidget; }

	// 델리게이트
protected:
	
	UFUNCTION()
	void OnUpdateCurrentWeapon(AWeaponBase* InCurrentWeapon);

	UFUNCTION()
	void OnUpdateCurrentGunInfo(AWeaponBase* InCurrentWeapon);

	UFUNCTION()
	void OnUpdateCurrentGunAmmo(int32 InCurrentAmmo);
		UFUNCTION()
        void OnUpdatePlayerHUDAmmoSlate(FGameplayTag FireMode, int32 CurrentAmmo, int32 TotalAmmo);
    
        UFUNCTION()
        void OnWeaponEquipped();
    
        UFUNCTION()
        void OnWeaponUnequipped();

		UFUNCTION()
		void OnUpdateWeaponFireMode(FGameplayTag FireMode);

		UFUNCTION()
		void OnUpdateMaxAmmoCount(int32 Val);

	


	
	
	
};
