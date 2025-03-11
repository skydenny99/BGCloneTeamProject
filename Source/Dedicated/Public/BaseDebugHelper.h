#pragma once
#include "GameFramework/PlayerState.h"

namespace Debug
{
	static void Print(const FString& Msg, const FColor& Color = FColor::MakeRandomColor(), int32 InKey = -1, const float TimeToDisplay = 7.0f)
	{
		if (GEngine)
		{
			//GEngine->AddOnScreenDebugMessage(InKey, TimeToDisplay, Color, Msg);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
		}
	}

	static void PrintError(const FString& Msg, const float TimeToDisplay = 7.0f)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Red, Msg);

			UE_LOG(LogTemp, Error, TEXT("%s"), *Msg);
		}
	}

	static void Print(const FString& FloatTitle, float FloatValueToPrint, const FColor& Color = FColor::MakeRandomColor(), int32 Inkey = -1)
	{
		if (GEngine)
		{
			const FString FinalMsg = FloatTitle + TEXT(": ") + FString::SanitizeFloat(FloatValueToPrint);
			// GEngine->AddOnScreenDebugMessage(Inkey, 7.0f, Color, FinalMsg);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMsg);
		}
	}
	
	static void PrintWithClientID(const UWorld* World, const FString& Msg, const FColor& Color = FColor::MakeRandomColor(), int32 InKey = -1, const float TimeToDisplay = 7.0f)
	{
		if (World)
		{
			if (APlayerController* PC = World->GetFirstPlayerController())
			{
				if (APlayerState* State = PC->GetPlayerState<APlayerState>())
				{
					FString FinalMsg = FString::Printf(TEXT("Player ID: %d, %s"), State->GetPlayerId(), *Msg) ;
					if (GEngine)
					{
						// GEngine->AddOnScreenDebugMessage(InKey, TimeToDisplay, Color, Msg);

						UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
					}
					return;
				}
			}
		}
		UE_LOG(LogTemp, Error, TEXT("%s"), *FString("No World Context! Use default Print"));
		Print(Msg, Color, InKey, TimeToDisplay);
	}
	
	static void PrintWithClientID(const UWorld* World, const FString& FloatTitle, float FloatValueToPrint, const FColor& Color = FColor::MakeRandomColor(), int32 InKey = -1, const float TimeToDisplay = 7.0f)
	{
		if (World)
		{
			if (APlayerController* PC = World->GetFirstPlayerController())
			{
				if (APlayerState* State = PC->GetPlayerState<APlayerState>())
				{
					if (GEngine)
					{
						FString FinalMsg = FloatTitle + TEXT(": ") + FString::SanitizeFloat(FloatValueToPrint);
						FinalMsg = FString::Printf(TEXT("Player ID: %d, %s"), State->GetPlayerId(), *FinalMsg) ;
						// GEngine->AddOnScreenDebugMessage(InKey, TimeToDisplay, Color, FinalMsg);

						UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMsg);
					}
					return;
				}
			}
		}
		UE_LOG(LogTemp, Error, TEXT("%s"), *FString("No World Context! Use default Print"));
		Print(FloatTitle, FloatValueToPrint, Color, InKey);
	}
}
