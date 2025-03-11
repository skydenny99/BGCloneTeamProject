// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillFeedWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DEDICATED_API UKillFeedWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 서서히 나타나는 애니메이션 플레이 함수*/
	void PlayKillAnimation();

	/** 죽인 사람의 닉네임 설정 함수 */
	void SetPlayerText(const FText& _SourceName);

	/** 죽은 사람의 닉네임 설정 함수*/
	void SetEnemyText(const FText& _TargetName);

	/** 무엇으로 죽였는지 설정 함수*/
	void SetWeaponIcon(UTexture2D* _SourceName);
	
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerText;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_Weapon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EnemyText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* KillAnim;
};
