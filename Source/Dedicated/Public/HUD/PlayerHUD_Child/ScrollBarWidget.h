// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollBarWidget.generated.h"

class UImage;
class UTextBlock;
class UScrollBox;
/**
 *  인벤토리에서 주변, 인벤토리의 Scroll UI
 */
UCLASS()
class DEDICATED_API UScrollBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ScrollBarWidget",meta=(AllowPrivateAccess=true))
	bool bIsAround = true;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Title;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Category_One;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Category_Two;

	UPROPERTY(meta = (BindWidget))
	UImage* Img_Scroll_BG;
	
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBox;

protected:
	// 무엇이든 드래스 상태로 해당 위젯에 들어왔을 시 호출되는 함수 = BeginOverlap
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	// 무엇이든 드래스 상태로 해당 위젯에 들어왔을 시 호출되는 함수 = EndOverlap
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	// 드래그 후 해당 위젯에 투척하면 호출 되는 함수
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	
public:
	FORCEINLINE UScrollBox* GetScrollBox() { return ScrollBox; }
};
