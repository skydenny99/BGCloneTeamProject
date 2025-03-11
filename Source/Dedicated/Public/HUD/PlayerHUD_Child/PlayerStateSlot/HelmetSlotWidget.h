// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HelmetSlotWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class DEDICATED_API UHelmetSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// BeginPlay 이벤트 초기화
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HelmetBar;

private:
	// 헬멧의 현재 내구도(0 ~ 100)
	float CurrentDurability = 100.0f;

	// 헬멧 레벨(0 : 레벨, 1 : 레벨, 2 : 레벨, 3 : 레벨)
	int32 HelmetLevel=0;

	// 헬멧의 최대 내구도
	float MaxDurability = 100.0f;

public:
	// 헬멧 내구도 업데이트
	void UpdateHelmetDurability(float _NewDurability);

	// 헬멧 레벨 업데이트
	void UpdateHelmetBarTexture(int32 _NewHelmetLevel);

	// 디폴트 상태로 초기화
	void SetToDefaultState();

private:
	// 헬멧 레벨에 따른 텍스쳐 경로 반환
	FString GetHelmetTexturePath(int32 _HelmetLevel) const;

	// ProgressBar를 빨간색으로 변경(내구도 0일 때)
	void SetHelmetBarToCriticalState();	
	
};
