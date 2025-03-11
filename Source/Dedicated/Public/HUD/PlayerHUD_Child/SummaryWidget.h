// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SummaryWidget.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DEDICATED_API USummaryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void Setup(const bool _Is1st, const FString& OwnerName = TEXT(""));

	UFUNCTION()
	void OnQuitGame();
	
	
private:
	/** 2번 호출 방지용 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup, meta = (AllowPrivateAccess = "true"))
	bool bIsLock;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_PlayerName;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_RankDesc;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Rank;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_TotalPeople;
	
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_GameExit;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Start_1st_Summary;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Start_Summary;
	
};
