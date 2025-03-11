// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "PossessWeaponSlotWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class DEDICATED_API UPossessWeaponSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetupImage(UTexture2D* _Texture);
	void SetupNumber(const int32 _Number);
	void ToggleAlpha(const bool _Alpha);
private:
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Weapon;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Number;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<int32, UTexture2D*> NumberMap;
};
