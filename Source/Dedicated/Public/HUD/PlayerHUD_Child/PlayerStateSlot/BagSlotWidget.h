// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BagSlotWidget.generated.h"

class UProgressBar;

/**
 * 가방 게이지와 레벨 업데이트를 처리하는 위젯 클래스
 */
UCLASS()
class DEDICATED_API UBagSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// BeginPlay 이벤트 초기화
	virtual void NativeConstruct() override;

private:
	// 가방에 담긴 아이템들의 무게 리스트
	//TArray<float> ItemWeights;
	
	// 현재 가방 게이지 값
	float CurrentBagBar=0.0f;

	// 가방 레벨(0 : 없음, 1 : 1렙, 2 : 2렙, 3 : 3렙)
	int32 BagLevel = 0;

	// 최대 가방 무게
	//float MaxBagWeight = 50.0f; // 기본값 : 가방 없는 상태 무게 값

public:
	// 바인딩된 ProgressBar(가방 게이지 표시)
	UPROPERTY(meta = (BindWidget))
	UProgressBar* BagBar;

public:
	// Getter : 현재 가방 무게 반환
	//float GetCurrentBagWeight() const;
	
	// Setter 함수 : ProgressBar 게이지 업데이트
	//void SetUpdateBagBar(float NewBagBar);

	// Setter 함수 : 가방 레벨 업데이트
	//void SetBagLevel(int32 NewBagLevel);

	// 아이템 추가 및 제거
	//void AddItem(float ItemWeight);
	//void RemoveItem(float ItemWeight);

	// ProgressBar 업데이트
	void UpdateBagGuage(float _Min, float _Max, float _CurrentBagWeight);

	// 가방 레벨에 따른 Texture 설정
	void UpdateBagBarTexture(int32 _BagLevel);

	// 디폴트 상태 초기화
	void SetToDefaultState();
	
private:
	// ProgressBar 숨김/표시
	//void SetBagBarVisibility(bool bIsVisible);
	
	// 레벨에 따른 경로를 반환하는 헬퍼 함수
	FString GetBagTexturePath(int32 _BagLevel) const;
	
public:	
	// Getter : 현재 가방 레벨 반환
	//FORCEINLINE int32 GetBagLevel() const { return BagLevel; }

	// Getter : 최대 무게 변환
	//FORCEINLINE float GetMaxBagWeight() const { return MaxBagWeight; }

	// Getter : 현재 게이지 반환
	//FORCEINLINE float GetUpdateBagBar() const { return CurrentBagBar; }
	
};