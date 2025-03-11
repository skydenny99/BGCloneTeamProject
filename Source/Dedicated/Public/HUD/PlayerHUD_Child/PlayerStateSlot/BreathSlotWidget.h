// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BreathSlotWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class DEDICATED_API UBreathSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UImage* IMG_Breath;
	
	
};
