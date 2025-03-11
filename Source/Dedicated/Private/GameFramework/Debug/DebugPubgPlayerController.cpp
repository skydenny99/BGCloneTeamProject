// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Debug/DebugPubgPlayerController.h"

#include "BaseDebugHelper.h"
#include "Characters/PubgBaseCharacter.h"
#include "Components/BaseWeaponManageComponent.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "GameFramework/PubgGameMode.h"
#include "GameFramework/PubgGameState.h"
#include "GameFramework/Debug/PubgCheatManager.h"
#include "Weapon/WeaponBase.h"


class UBaseWeaponManageComponent;

void ADebugPubgPlayerController::AddCheats(bool bForce)
{
	Super::AddCheats(true);
}

ADebugPubgPlayerController::ADebugPubgPlayerController()
{
	CheatClass = UPubgCheatManager::StaticClass();
}

void ADebugPubgPlayerController::Server_RemoveMagazine_Implementation()
{
	if (APubgBaseCharacter* PubgBaseCharacter = Cast<APubgBaseCharacter>(GetPawn()))
	{
		if (UEquipManageComponent* EquipManager = PubgBaseCharacter->GetEquipManageComponent())
		{
			if (AWeaponBase* Weapon = EquipManager->GetCurrentWeapon())
			{
				Weapon->UnequipWeaponParts(BaseGameplayTags::Item_Parts_Magazine);
			}
		}
	}
}

void ADebugPubgPlayerController::Server_CheckPartsAvailable_Implementation(int32 InItemID)
{
	if (APubgBaseCharacter* PubgBaseCharacter = Cast<APubgBaseCharacter>(GetPawn()))
	{
		if (UEquipManageComponent* EquipManager = PubgBaseCharacter->GetEquipManageComponent())
		{
			if (AWeaponBase* Weapon = EquipManager->GetCurrentWeapon())
			{
				const bool IsAvailable = Weapon->IsEquippableParts(InItemID);
				Debug::Print(IsAvailable ? "Yes" : "No");
			}
		}
	}
}

void ADebugPubgPlayerController::Server_RemoveSight_Implementation()
{
	if (APubgBaseCharacter* PubgBaseCharacter = Cast<APubgBaseCharacter>(GetPawn()))
	{
		if (UEquipManageComponent* EquipManager = PubgBaseCharacter->GetEquipManageComponent())
		{
			if (AWeaponBase* Weapon = EquipManager->GetCurrentWeapon())
			{
				Weapon->UnequipWeaponParts(BaseGameplayTags::Item_Parts_Sight);
			}
		}
	}
}

void ADebugPubgPlayerController::Server_AddSight_Implementation(int32 InItemID)
{
	if (APubgBaseCharacter* PubgBaseCharacter = Cast<APubgBaseCharacter>(GetPawn()))
	{
		if (UEquipManageComponent* EquipManager = PubgBaseCharacter->GetEquipManageComponent())
		{
			if (AWeaponBase* Weapon = EquipManager->GetCurrentWeapon())
			{
				Weapon->EquipWeaponParts(InItemID);
			}
		}
	}
}

void ADebugPubgPlayerController::Server_RemoveMuzzle_Implementation()
{
	if (APubgBaseCharacter* PubgBaseCharacter = Cast<APubgBaseCharacter>(GetPawn()))
	{
		if (UEquipManageComponent* EquipManager = PubgBaseCharacter->GetEquipManageComponent())
		{
			if (AWeaponBase* Weapon = EquipManager->GetCurrentWeapon())
			{
				Weapon->UnequipWeaponParts(BaseGameplayTags::Item_Parts_Muzzle);
			}
		}
	}
}

void ADebugPubgPlayerController::SetBluezoneRound_Implementation(int32 InNewRound)
{
	if (APubgGameState* GameState = GetWorld()->GetGameState<APubgGameState>())
	{
		GameState->UpdateBlueZone(InNewRound);
	}
}

void ADebugPubgPlayerController::Server_CheckCurrentWeapon_Implementation()
{
	if (APubgBaseCharacter* PubgBaseCharacter = Cast<APubgBaseCharacter>(GetPawn()))
	{
		if (UEquipManageComponent* EquipManager = PubgBaseCharacter->GetEquipManageComponent())
		{
			Debug::Print(EquipManager->GetCurrentWeapon()->GetActorNameOrLabel());
		}
	}
}

void ADebugPubgPlayerController::SetBluezoneRadius_Implementation(float NewRadius, float InPeriod)
{
	if (APubgGameState* GameState = GetWorld()->GetGameState<APubgGameState>())
	{
		//GameState->UpdateBlueZoneRadius(NewRadius, InPeriod);
	}
}

void ADebugPubgPlayerController::SetBluezoneLevel_Implementation(int32 NewLevel)
{
	if (APubgGameState* GameState = GetWorld()->GetGameState<APubgGameState>())
	{
		GameState->UpdateBlueZoneLevel(NewLevel);
	}
}

void ADebugPubgPlayerController::StartGame_Implementation()
{
	if (APubgGameMode* GameMode = Cast<APubgGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->StartMatch();
	}
}

