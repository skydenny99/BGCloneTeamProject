// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/KillFeedWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"


void UKillFeedWidget::PlayKillAnimation()
{
	PlayAnimation(KillAnim);
}

void UKillFeedWidget::SetPlayerText(const FText& _SourceName)
{
	PlayerText->SetText(_SourceName);
}

void UKillFeedWidget::SetEnemyText(const FText& _TargetName)
{
	EnemyText->SetText(_TargetName);
}

void UKillFeedWidget::SetWeaponIcon(UTexture2D* _SourceName)
{
	IMG_Weapon->SetBrushFromTexture(_SourceName);
}
