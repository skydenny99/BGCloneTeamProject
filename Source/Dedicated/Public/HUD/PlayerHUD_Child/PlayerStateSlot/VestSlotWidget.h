// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VestSlotWidget.generated.h"

class UProgressBar;
/**
 * 조끼 슬롯 위젯
 */
UCLASS()
class DEDICATED_API UVestSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// BeginPlay 이벤트 초기화
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* VestBar;

private:	
	// 조끼 착용 여부
	//bool bIsVestEquipped=false;

	// 조끼 내구도(기본 값 : 100)
	float CurrentDurability=100.0f;

	// 조끼 레벨 (0 : 없음, 1 : 1레벨, 2 : 2레벨, 3 : 3레벨)
	int32 VestLevel = 0;

	// 조끼 최대 내구도
	float MaxDurability=100.0f;

	// 추가 무게(기본 값 : 0)
	//float VestWeightBonus=50.0f;

public:
	// 조끼 내구도 업데이트
	void UpdateVestDurability(float _NewDurability);

	// 조끼 텍스쳐 레벨 업데이트
	void UpdateVestBarTexture(int32 _NewVestLevel);

	// 디폴트 상태로 초기화
	void SetToDefaultState();	
	
	// 조끼 레벨 및 착용 설정
	//void SetVestLevel(int32 NewVestLevel);
	
	// Setter : 조끼 착용 여부
	// void SetVestEquipped(bool bEquipped);

	// 데미지를 받아 내구도를 감소시키는 함수
	//void ApplyDamageToVest(float Damage);

private:
	// ProgressBar 및 텍스쳐 업데이트
	// void UpdateVestBar();
	
	// 조끼 레벨에 따라 ProgressBar 텍스쳐 업데이트
	// void UpdateVestBarTexture();

	// 레벨에 따른 텍스쳐 경로를 반환하는 헬퍼 함수
	FString GetVestTexturePath(int32 _VestLevel) const;

	// ProgressBar를 빨간색으로 변경(내구도 0일 때)
	void SetVestBarToCriticalState();
		
};
