// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Debug/PubgCheatManager.h"

#include "BaseDebugHelper.h"
#include "OnlineSubsystemUtils.h"
#include "Characters/PubgCharacter.h"
#include "Components/BasePickUpComponent.h"
#include "Components/BaseWeaponManageComponent.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "Components/Renewal/InventoryManageComponent.h"
#include "Components/Renewal/PickUpManageComponent.h"
#include "GameFramework/PubgGameMode.h"
#include "GameFramework/PubgGameState.h"
#include "GameFramework/PubgPlayerState.h"
#include "GameFramework/Debug/DebugPubgPlayerController.h"
#include "GameFramework/Lobby/PubgLobbyPlayerController.h"
#include "GameFramework/Lobby/PubgLobbySubsystem.h"
#include "Weapon/WeaponBase.h"

#define PLAYER_CONTROLLER(X) \
	PlayerController = Cast<ADebugPubgPlayerController>(GetPlayerController()); \
	if (PlayerController)\
	{\
		(X); \
	}


void UPubgCheatManager::Start()
{
	if (APubgLobbyPlayerController* LobbyPlayer = Cast<APubgLobbyPlayerController>(GetPlayerController()))
	{
		// LobbyPlayer->StartGame();
	}
}


void UPubgCheatManager::ForceGarbageCollection()
{
	GEngine->ForceGarbageCollection(true);
}

void UPubgCheatManager::AlivePlayerCount()
{
	if (APubgGameState* GameState = Cast<APubgGameState>(GetWorld()->GetGameState()))
	{
		Debug::Print("Current Alive Player", GameState->GetAlivePlayerCount());
	}
}

void UPubgCheatManager::KillCount()
{
	PlayerController = Cast<ADebugPubgPlayerController>(GetPlayerController()); \
	if (PlayerController)
	{
		if (APubgPlayerState* PlayerState = PlayerController->GetPlayerState<APubgPlayerState>())
		{
			Debug::Print("Current Player Kill Count", PlayerState->GetKillCount());
		}
	}
	
}

void UPubgCheatManager::StartMatch()
{
	PLAYER_CONTROLLER(PlayerController->StartGame())
}


void UPubgCheatManager::Cheat_AcquireItem(int32 InItemID, int32 InCount)
{
	PlayerController = Cast<ADebugPubgPlayerController>(GetPlayerController());
	if (GetPlayerController())
	{
		if (APubgBaseCharacter* PubgBaseCharacter = Cast<APubgBaseCharacter>(PlayerController->GetPawn()))
		{
			if (PubgBaseCharacter && PubgBaseCharacter->GetInventoryComponent())
			{
				Debug::Print("Cheat_AcquireItem");
				int32 OutCount = 0;
				PubgBaseCharacter->GetInventoryComponent()->AcquireItem(InItemID,InCount,OutCount);
			}
		}
	}
	
}

void UPubgCheatManager::Cheat_UseItem(int32 InItemID)
{
	PlayerController = Cast<ADebugPubgPlayerController>(GetPlayerController()); 
	if (PlayerController)
	{
		if (APubgBaseCharacter* PubgBaseCharacter = Cast<APubgBaseCharacter>(PlayerController->GetPawn()))
		{
			
			if (PubgBaseCharacter && PubgBaseCharacter->GetInventoryComponent())
			{
				Debug::Print("Cheat_UseItem");
				PubgBaseCharacter->GetInventoryComponent()->UseItem(InItemID, 1);
			}
		}
	}
	
}


void UPubgCheatManager::Cheat_DiscardItem(int32 InItemID, int32 InCount)
{
	PlayerController = Cast<ADebugPubgPlayerController>(GetPlayerController()); 
	if (PlayerController)
	{
		if (APubgBaseCharacter* PubgBaseCharacter = Cast<APubgBaseCharacter>(PlayerController->GetPawn()))
		{
			if (PubgBaseCharacter && PubgBaseCharacter->GetInventoryComponent())
			{
				Debug::Print("Cheat_DiscardItem");
				PubgBaseCharacter->GetInventoryComponent()->DiscardItem(InItemID,InCount);
			}
		}
	}

	
}

void UPubgCheatManager::Cheat_PrintPickUp()
{
	PlayerController = Cast<ADebugPubgPlayerController>(GetPlayerController()); 
	if (PlayerController)
	{
		if (APubgBaseCharacter* PubgBaseCharacter = Cast<APubgBaseCharacter>(PlayerController->GetPawn()))
		{
			if (UPickUpManageComponent* PickUpComponent = PubgBaseCharacter->GetPickUpManageComponent())
			{
				//PickUpComponent->Debug_ShowPickUpCandidates();
			}
		}
	}
}

void UPubgCheatManager::Cheat_TryPickUp(int32 InIndex)
{
	PlayerController = Cast<ADebugPubgPlayerController>(GetPlayerController()); 
	if (PlayerController)
	{
		if (APubgBaseCharacter* PubgBaseCharacter = Cast<APubgBaseCharacter>(PlayerController->GetPawn()))
		{
			if (UPickUpManageComponent* PickUpComponent = PubgBaseCharacter->GetPickUpManageComponent())
			{
				//PickUpComponent->Debug_PickUpCandidateByIndex(InIndex);
			}
		}
	}
}

void UPubgCheatManager::Cheat_Unequip(int32 InIndex)
{
	PlayerController = Cast<ADebugPubgPlayerController>(GetPlayerController()); 
	if (PlayerController)
	{
		if (APubgBaseCharacter* PubgBaseCharacter = Cast<APubgBaseCharacter>(PlayerController->GetPawn()))
		{
			if (UPickUpManageComponent* PickUpComponent = PubgBaseCharacter->GetPickUpManageComponent())
			{
				PickUpComponent->Server_DiscardWeapon(BaseGameplayTags::Item_Slot_Primary);
			}
		}
	}
}

void UPubgCheatManager::Cheat_DiscardPickUp(int32 InItemID, int32 InCount)
{
	PlayerController = Cast<ADebugPubgPlayerController>(GetPlayerController()); 
	if (PlayerController)
	{
		if (APubgBaseCharacter* PubgBaseCharacter = Cast<APubgBaseCharacter>(PlayerController->GetPawn()))
		{
			if (UPickUpManageComponent* PickUpComponent = PubgBaseCharacter->GetPickUpManageComponent())
			{
				//PickUpManage->Server_DiscardPickUp(InItemID,InCount);
			}
		}
	}
}

void UPubgCheatManager::Cheat_AddSight(int32 InItemID)
{
	PLAYER_CONTROLLER(PlayerController->Server_AddSight(InItemID))
}

void UPubgCheatManager::Cheat_RemoveSight()
{
	PLAYER_CONTROLLER(PlayerController->Server_RemoveSight())
}

void UPubgCheatManager::Cheat_RemoveMuzzle()
{
	PLAYER_CONTROLLER(PlayerController->Server_RemoveMuzzle())
}

void UPubgCheatManager::Cheat_RemoveMagazine()
{
	PLAYER_CONTROLLER(PlayerController->Server_RemoveMagazine())
}

void UPubgCheatManager::Cheat_CheckPartsAvailable(int32 InItemID)
{
	PLAYER_CONTROLLER(PlayerController->Server_CheckPartsAvailable(InItemID))
}

void UPubgCheatManager::Cheat_SetBluezoneLevel(int32 InLevel)
{
	PLAYER_CONTROLLER(PlayerController->SetBluezoneLevel(InLevel))
}

void UPubgCheatManager::Cheat_SetBluezoneRadius(float InRadius, float InPeriod)
{
	PLAYER_CONTROLLER(PlayerController->SetBluezoneRadius(InRadius, InPeriod))
}

void UPubgCheatManager::Cheat_SetBluezoneRound(int32 InRound)
{
	PLAYER_CONTROLLER(PlayerController->SetBluezoneRound(InRound))
}

void UPubgCheatManager::Cheat_WeaponManage()
{
	PlayerController = Cast<ADebugPubgPlayerController>(GetPlayerController()); 
	if (PlayerController)
	{
		if (APubgBaseCharacter* PubgBaseCharacter = Cast<APubgBaseCharacter>(PlayerController->GetPawn()))
		{
			if (UEquipManageComponent* EquipManageComponent = PubgBaseCharacter->GetEquipManageComponent())
			{
				Debug::Print(EquipManageComponent->GetCurrentWeapon()->GetActorNameOrLabel());
			}
		}
		PlayerController->Server_CheckCurrentWeapon();
	}
}
