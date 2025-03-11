// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CompassWidget.generated.h"

class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class DEDICATED_API UCompassWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UImage* CompassImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DirectionText;

protected:
	// 머티리얼 인스턴스를 저장할 변수
	UPROPERTY(Transient);
	UMaterialInstanceDynamic* CompassMaterial;

	// 업데이트 주기를 설정하는 타이머 핸들
	FTimerHandle TimerHandle;

	// 머티리얼 파라미터 이름
	FName ParameterName=TEXT("Offset");

	
	//float Yaw;
	
	virtual void NativePreConstruct() override;

	// 컴파스 업데이트 함수
	void UpdateCompass();

public:
	virtual void NativeConstruct() override;


public:
	FORCEINLINE	UImage* GetCompassImage() const {return CompassImage;}

	
	
	
	
};
