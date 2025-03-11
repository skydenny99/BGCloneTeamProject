// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PubgPlayerState.h"

#include "BaseDebugHelper.h"
#include "HUD/HUDBase.h"
#include "HUD/PlayerHUD/PlayerHUD.h"
#include "Kismet/GameplayStatics.h"

#include "Net/UnrealNetwork.h"


void APubgPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APubgPlayerState, PubgPlayerName)
	DOREPLIFETIME(APubgPlayerState, EliminatedPlayerName)
	DOREPLIFETIME(APubgPlayerState, KillCount)
	DOREPLIFETIME(APubgPlayerState, PlayerGunID)
}

void APubgPlayerState::OnRep_UpdateKillCount()
{
	if (APlayerController* PlayerController = GetPlayerController())
	{
		if (AHUDBase* HUDBase = Cast<AHUDBase>(PlayerController->GetHUD()))
		{
			if (UPlayerHUD* PlayerHUD = HUDBase->PlayerHUD)
			{
				if (PlayerHUD->OnKillCount.IsBound())
				{
					PlayerHUD->OnKillCount.Execute(KillCount);
				}

				if (PlayerHUD->OnKillFeed.IsBound())
				{
					PlayerHUD->OnKillFeed.Execute(PubgPlayerName,EliminatedPlayerName,PlayerGunID);
					Debug::Print(FString::Printf(TEXT("Source Name : %s, Target Name : %s"),*GetName(), *EliminatedPlayerName));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayerHUD not Cast!!!"));
			}
		}
	}
}