// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interact.generated.h"

class UBackgroundBlur;
class UTextBlock;
class UImage;
/**
 *  근처 아이템 바라 볼 시 활성화 되는 UI
 */
UCLASS()
class DEDICATED_API UInteract : public UUserWidget
{
	GENERATED_BODY()

private:

	/** 무슨 버튼을 누를건지 표시하는 블럭 = 기본 F */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Slot_Text;

	/** 현재 바라보고 있는 아이템의 DisplayName이 기입될 변수 */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Dest_Text;

	/** 블러에 색상을 입힐 용도*/
	UPROPERTY(meta = (BindWidget))
	UImage* BlurColor_Img;

	/** 블러 */
	UPROPERTY(meta = (BindWidget))
	UBackgroundBlur* Blur_BG;

public:
	/** F키를 변경할 예정이면 해당 변수를 통해 변경 */
	UFUNCTION(BlueprintCallable)
	void SetSlotText(const FText InputKeyText);

	/** 전달받은 변수를 설정
	 * @param Dest ItemDisplayName
	 */
	UFUNCTION(BlueprintCallable)
	void SetDestText(const FText Dest, const bool bisItem = true);


public:
	/** 전달받은 Text의 크기만큼 Widget 조절하기 위해 현재 설정된 Text 길이 반환 */
	float GetLengthDescText();
	
};
