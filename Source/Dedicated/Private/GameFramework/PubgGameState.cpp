// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PubgGameState.h"
#include "HUD/HUDBase.h"
#include "HUD/PlayerHUD/PlayerHUD.h"

#include "BaseDebugHelper.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Bluezone/BaseBluezone.h"
#include "Characters/PubgBaseCharacter.h"
#include "HUD/PlayerHUD_Child/SummaryWidget.h"
#include "Net/UnrealNetwork.h"


APubgGameState::APubgGameState()
{
	AlivePlayerCount = 0;
	
	bReplicates = true;
	NetUpdateFrequency = 10.f;

	// 블루존 스폰위치 기본 세팅
	SpawnLocation = FVector(-10049.0f, -5265.0f, 140.0f);
	SpawnRotation = FRotator(0.0f, 0.0f, 0.0f); 
}


void APubgGameState::UpdateCurrentBlueZone()
{
	if (BlueZone)
	{
		const float PassedTime = GetServerWorldTimeSeconds() - BlueZoneStartTime;
		const float Alpha = PassedTime / FMath::Max( BlueZonePeriod, 1.f);
		float NewRadius = FMath::Lerp(CurrentBlueZoneRadius, TargetBlueZoneRadius, Alpha);
		FVector NewLocation = FMath::Lerp(CurrentBlueZoneLocation, TargetBlueZoneLocation, Alpha);
		if (Alpha >= 1.f)
		{
			NewRadius = TargetBlueZoneRadius;
			CurrentBlueZoneRadius = NewRadius;
			NewLocation = TargetBlueZoneLocation;
			CurrentBlueZoneLocation = NewLocation;
			
			const float DelayPeriod = 90.f;
			GetWorldTimerManager().SetTimer(BlueZoneTimer, this, &APubgGameState::StartNextBlueZone, DelayPeriod, false);
		}
		BlueZone->SetBlueZoneInfo(NewLocation, NewRadius);
	}
}

void APubgGameState::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	if (HasAuthority() && BlueZoneClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		BlueZone = GetWorld()->SpawnActor<ABaseBluezone>
		(
			BlueZoneClass,
			SpawnLocation,
			SpawnRotation,
			SpawnParams);
		CurrentBlueZoneRadius = 100000.f;
		CurrentBlueZoneLocation = SpawnLocation;
		BlueZone->SetBlueZoneInfo(CurrentBlueZoneLocation, CurrentBlueZoneRadius);
		const float DelayPeriod = 90.f;
		GetWorldTimerManager().SetTimer(BlueZoneTimer, this, &APubgGameState::StartNextBlueZone, DelayPeriod, false);
	}
}

void APubgGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APubgGameState, AlivePlayerCount);
	DOREPLIFETIME(APubgGameState, TotalPlayerCount);
	DOREPLIFETIME(APubgGameState, BlueZoneRound);
}


bool APubgGameState::GetGameElapsedSecs(int32& OutGameElapsedSecs) const
{
	OutGameElapsedSecs = ElapsedTime;
	return ElapsedTime > 0;
}


void APubgGameState::UpdateBlueZoneLevel(int32 BlueZoneLevel)
{
	if (HasAuthority())
	{
		for (auto& PlayerState : PlayerArray)
		{
			if (PlayerState)
			{
				if (APubgBaseCharacter* Character = Cast<APubgBaseCharacter>(PlayerState->GetPawn()))
				{
					Character->GetBaseAbilitySystemComponent()->SetActiveGameplayEffectLevelByTag(BaseGameplayTags::State_Debuff_BluezoneDamage, BlueZoneLevel);
				}
			}
		}
	}
	
}

void APubgGameState::UpdateBlueZone(int32 InBlueZoneRound)
{
	if (HasAuthority() && BlueZone)
	{
		const float Period = 30.f; // TEMP
		const float NewRadius = CurrentBlueZoneRadius / 2; // TEMP
		const float DeltaRadius = FMath::RandRange(0.f, CurrentBlueZoneRadius - NewRadius);
		const FVector RandomUnit = FVector(FMath::RandRange(-1.f, 1.f), FMath::RandRange(-1.f, 1.f), 0).GetSafeNormal();
		BlueZonePeriod = Period;
		TargetBlueZoneLocation = CurrentBlueZoneLocation + RandomUnit * DeltaRadius;
		TargetBlueZoneRadius = NewRadius;
		BlueZoneRound = InBlueZoneRound;
		BlueZoneStartTime = GetServerWorldTimeSeconds();
		GetWorldTimerManager().SetTimer(BlueZoneTimer, this, &APubgGameState::UpdateCurrentBlueZone, 0.1f, true);
		UpdateBlueZoneLevel(InBlueZoneRound);
	}
}

void APubgGameState::OnRep_PlayerCount()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (AHUDBase* BaseHUD = Cast<AHUDBase>(PlayerController->GetHUD()))
		{
			BaseHUD->PlayerHUD->UpdateCountSurvivors(AlivePlayerCount);

			if (AlivePlayerCount <= 1)
			{
				PlayerController->SetInputMode(FInputModeGameAndUI());
				PlayerController->SetShowMouseCursor(true);
				BaseHUD->GetSummaryWidget()->SetVisibility(ESlateVisibility::Visible);
				BaseHUD->GetSummaryWidget()->Setup(true);
			}
		}
	}
}
