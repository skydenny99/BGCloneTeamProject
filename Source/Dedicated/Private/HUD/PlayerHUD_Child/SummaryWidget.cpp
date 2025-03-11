// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/SummaryWidget.h"

#include "BaseDebugHelper.h"
#include "Characters/PubgCharacter.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/PubgGameState.h"
#include "GameFramework/PubgPlayerState.h"
#include "Kismet/KismetSystemLibrary.h"


void USummaryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_GameExit)
	{
		Btn_GameExit->OnClicked.AddDynamic(this, &USummaryWidget::OnQuitGame);
	}
}

void USummaryWidget::Setup(const bool _Is1st, const FString& OwnerName)
{
	// 2번 호출 방지용, 
	if (bIsLock) return;
	APubgGameState* GS = GetWorld()->GetGameState<APubgGameState>();
	if (!GS) return;

	AActor* OwnerActor = GetOwningPlayerPawn();
	if (!OwnerActor) return;
	
	//FString TempName = OwnerActor->GetActorLabel();
	if (!_Is1st)
	{
		Text_PlayerName->SetText(FText::FromString(OwnerName));
		Debug::Print("Owner Player Name : " + OwnerName);
	}
	else
	{
		APubgPlayerState* PS = GetOwningPlayer()->GetPlayerState<APubgPlayerState>();
		if (!PS) return;
		Text_PlayerName->SetText(FText::FromString(PS->GetSourcePlayerName()));
		Debug::Print("Owner Player Name : " + PS->GetSourcePlayerName());
	}
	
	
	FText TempRankDesc = 
	_Is1st ? FText::FromString(TEXT("이겼닭! 오늘은 치킨이닭!")) : FText::FromString(TEXT("그럴 수 있어. 이런 날도 있는 거지 머."));
	
	Text_RankDesc->SetText(TempRankDesc);

	Text_Rank->SetText(FText::FromString(FString::Printf(TEXT("#%d"),GS->GetAlivePlayerCount())));
	Text_TotalPeople->SetText(FText::FromString(FString::Printf(TEXT("/ %d"),GS->GetTotalPlayerCount())));
	
	_Is1st ? PlayAnimation(Start_1st_Summary) : PlayAnimation(Start_Summary);

	bIsLock = true;
}

void USummaryWidget::OnQuitGame()
{
	if (GetWorld())
	{
		UKismetSystemLibrary::QuitGame(GetWorld(),nullptr, EQuitPreference::Quit, false);
	}
}
