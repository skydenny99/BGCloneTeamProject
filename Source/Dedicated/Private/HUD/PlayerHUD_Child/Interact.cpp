// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD_Child/Interact.h"

#include "BaseDebugHelper.h"
#include "Components/BackgroundBlur.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UInteract::SetSlotText(const FText InputKeyText)
{
	Slot_Text->SetText(InputKeyText);
}

void UInteract::SetDestText(const FText Dest, const bool bisItem)
{
	// Set Text
	if (bisItem)
	{
		FString DestString = FString::Printf(TEXT("%s 줍기"), *Dest.ToString());
		Dest_Text->SetText(FText::FromString(DestString));
	}
	else
	{
		Dest_Text->SetText(Dest);
	}
}

float UInteract::GetLengthDescText()
{
	return Dest_Text->GetText().ToString().Len();
}
