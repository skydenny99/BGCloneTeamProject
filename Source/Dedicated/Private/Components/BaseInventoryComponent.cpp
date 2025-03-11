// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BaseInventoryComponent.h"

#include "Characters/PubgCharacter.h"
#include "Components/BaseInteractComponent.h"
#include "Components/BaseWeaponManageComponent.h"
#include "HUD/HUDBase.h"
#include "HUD/UI/InventoryMainWidget.h"


// Sets default values for this component's properties
UBaseInventoryComponent::UBaseInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}

// 아이템 획득 함수 (기본 반환 값)
bool UBaseInventoryComponent::AcquireItem(int32 InItemID, int32 InCount ,int32& OutRemainingCount)
{
	// 기본 동작 정의
	OutRemainingCount = 0;
	return false; // 아이템을 획득하지 못함 (기본)
}

void UBaseInventoryComponent::Client_AcquireItem_Implementation(int32 InItemID, int32 InCount)
{
	int32 RemainingAmount;
	AcquireItem(InItemID, InCount, RemainingAmount);
	if (APubgCharacter* Character = Cast<APubgCharacter>(GetOwner()))
	{
		Debug::Print(TEXT("UBaseInventoryComponent ----- Server_PickUp_Implementation"));
//		Character->GetWeaponManageComponent()->UpdateMaxAmmoCount(InItemID);
	}
}

// 아이템 버리기 함수
void UBaseInventoryComponent::DiscardItem(int32 InItemID, int32 InCount,int32& OutCount)
{
	// 기본 동작 정의
}

void UBaseInventoryComponent::Client_DiscardItem_Implementation(int32 InItemID, int32 InCount)
{
	int32 OutCount;
	DiscardItem(InItemID, InCount, OutCount);
}

// 아이템 사용 함수
bool UBaseInventoryComponent::UseItem(int32 InItemID)
{
	// 기본 동작 정의
	return false; // 아이템 사용 실패 (기본)
}

// 아이템 보유 여부 확인
bool UBaseInventoryComponent::HasItem(int32 InItemID, int32 InCount )
{
	// 기본 동작 정의
	return false; // 아이템을 보유하지 않음 (기본)
}

// 특정 총알 보유 여부 확인
bool UBaseInventoryComponent::ReloadBullet(EBulletType InBulletType,int32 InCount, int32& OutCount)
{
	// 기본 동작 정의
	return false; // 해당 총알을 보유하지 않음 (기본)
}

void UBaseInventoryComponent::HasBullet(EBulletType InBulletType, int32& OutCount)
{
	// 기본 동작 정의
}

void UBaseInventoryComponent::DebugInventoryPrint()
{
	
}