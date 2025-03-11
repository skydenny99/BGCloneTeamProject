// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/BaseFunctionLibrary.h"

#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BaseDebugHelper.h"
#include "Characters/PubgCharacter.h"
#include "GameFramework/Character.h"
#include "Subsystem/PubgDataTableManageSubsystem.h"

UBaseAbilitySystemComponent* UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(AActor* Actor)
{
	check(Actor);
	return CastChecked<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor));
}

void UBaseFunctionLibrary::AddPlaygameTagToActor(AActor* Actor, FGameplayTag Tag)
{
	UBaseAbilitySystemComponent* ASC = NativeGetBaseAbilitySystemComponentFromActor(Actor);
	if (ASC->HasMatchingGameplayTag(Tag) == false)
	{
		ASC->AddLooseGameplayTag(Tag);
	}
}

void UBaseFunctionLibrary::RemovePlayGameTagFromActor(AActor* Actor, FGameplayTag Tag)
{
	UBaseAbilitySystemComponent* ASC = NativeGetBaseAbilitySystemComponentFromActor(Actor);
	if (ASC->HasMatchingGameplayTag(Tag))
	{
		ASC->RemoveLooseGameplayTag(Tag);
	}
}


bool UBaseFunctionLibrary::NativeActorHasTag(AActor* Actor, FGameplayTag Tag)
{
	UBaseAbilitySystemComponent* ASC = NativeGetBaseAbilitySystemComponentFromActor(Actor);
	if (ASC == nullptr) return false;
	return ASC->HasMatchingGameplayTag(Tag);
}

bool UBaseFunctionLibrary::BP_NativeActorHasTag(AActor* Actor, FGameplayTag Tag)
{
	return (NativeActorHasTag(Actor, Tag));
}


bool UBaseFunctionLibrary::NativeActorHasAnyTags(AActor* Actor, FGameplayTagContainer Tags)
{
	UBaseAbilitySystemComponent* ASC = NativeGetBaseAbilitySystemComponentFromActor(Actor);
	if (ASC == nullptr) return false;
	return ASC->HasAnyMatchingGameplayTags(Tags);
}

FRandomStream UBaseFunctionLibrary::SetSeedWithActivationKey(UBaseGameplayAbility* Ability)
{
	int16 Seed = 0;
	if (Ability != nullptr)
	{
		Seed = Ability->GetCurrentActivationInfoRef().GetActivationPredictionKey().Current;
	}
	return FRandomStream(Seed);
}

float UBaseFunctionLibrary::GetRandomFloat(UBaseGameplayAbility* Ability)
{
	return SetSeedWithActivationKey(Ability).FRand();
}

float UBaseFunctionLibrary::GetRandomFloatRange(UBaseGameplayAbility* Ability, float Min, float Max)
{
	return SetSeedWithActivationKey(Ability).FRandRange(Min, Max);
}

FGameplayAbilityTargetDataHandle UBaseFunctionLibrary::MakeTargetDataHandleFromTransform(const FTransform& InTransform)
{
	FGameplayAbilityTargetData_LocationInfo* LocationInfo = new FGameplayAbilityTargetData_LocationInfo();
	LocationInfo->TargetLocation.LiteralTransform = InTransform;
	FGameplayAbilityTargetDataHandle Handle;
	Handle.Add(LocationInfo);
	return Handle;
}

FVector UBaseFunctionLibrary::MakeCharacterAimVector(ACharacter* PlayerCharacter)
{
	if (PlayerCharacter->IsLocallyControlled() == false) return FVector::ZeroVector;
	APubgCharacter* PubgCharacter = Cast<APubgCharacter>(PlayerCharacter);
	if (APlayerController* PC = PlayerCharacter->GetController<APlayerController>())
	{
		if (APlayerCameraManager* CM = PC->PlayerCameraManager)
		{
			if (!PubgCharacter->GetFPP_MODE())
			{
				const FVector Forward =CM->GetCameraLocation() + CM->GetActorForwardVector() * 10000.f; // 100m 기준
				FHitResult Hit;
				if (PlayerCharacter->GetWorld()->LineTraceSingleByChannel(Hit, CM->GetCameraLocation(),Forward, ECC_Visibility))
				{
					Debug::Print(FString::Printf(TEXT("CM->GetCameraLocation() : %ls"),*CM->GetCameraLocation().ToString()));
					Debug::Print(FString::Printf(TEXT("CM->GetCameraRotation() : %ls"),*CM->GetCameraRotation().ToString()));
					Debug::Print(FString::Printf(TEXT("Forward : %ls"),*Forward.ToString()));
					return Hit.Location;
				}
				return Forward;
			}
			else
			{
				const FVector Start =CM->GetCameraLocation() + CM->GetActorForwardVector() * 50.f; 
				const FVector Forward =CM->GetCameraLocation() + CM->GetActorForwardVector() * 10000.f; // 100m 기준
				FHitResult Hit;
				if (PlayerCharacter->GetWorld()->LineTraceSingleByChannel(Hit, Start,Forward, ECC_Visibility))
				{
					Debug::Print(TEXT("FPPMode"));
					Debug::Print(FString::Printf(TEXT("CM->GetCameraLocation() : %ls"),*CM->GetCameraLocation().ToString()));
					Debug::Print(FString::Printf(TEXT("CM->GetCameraRotation() : %ls"),*CM->GetCameraRotation().ToString()));
					Debug::Print(FString::Printf(TEXT("Forward : %ls"),*Forward.ToString()));
					
				
					return Hit.Location;
				
				}
				return Forward;
					
				
			}
			
		}
	}
	Debug::Print("Error: No Player controller", FColor::Red);
	return FVector::ZeroVector;
}

const FInventoryDataStruct* UBaseFunctionLibrary::GetItemData(int32 InItemID)
{
	if (UPubgDataTableManageSubsystem* DB = UPubgDataTableManageSubsystem::GetInstance())
	{
		const FInventoryDataStruct* ItemData = DB->FindRow<FInventoryDataStruct>(FName(FString::FromInt(InItemID)));
		return ItemData;
	}
	Debug::PrintError("No DB Subsystem");
	return nullptr;
}

const FGunSpec* UBaseFunctionLibrary::GetGunData(int32 InItemID)
{
	if (UPubgDataTableManageSubsystem* DB = UPubgDataTableManageSubsystem::GetInstance())
	{
		const FGunSpec* GunData = DB->FindRow<FGunSpec>(FName(FString::FromInt(InItemID)));
		return GunData;
	}
	Debug::PrintError("No DB Subsystem");
	return nullptr;
}
