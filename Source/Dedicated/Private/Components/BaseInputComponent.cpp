// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BaseInputComponent.h"

#include "GameFramework/Character.h"


void UBaseInputComponent::BindAttackAction(UDataAsset_PubgInputConfig* InputConfig, ACharacter* Target)
{
	check(InputConfig);
	BindActionValueLambda(InputConfig->AttackInputAction, ETriggerEvent::Started, [Target] (const FInputActionValue& Value)
	{
		UBaseFunctionLibrary::AddPlaygameTagToActor(Target, BaseGameplayTags::InputTag_Attack);
		if (APubgPlayerController* Controller = Target->GetController<APubgPlayerController>())
		{
			Controller->AddGameplayTagToControlledPawn(BaseGameplayTags::InputTag_Attack);
		}
	});
	
	BindActionValueLambda(InputConfig->AttackInputAction, ETriggerEvent::Completed, [Target] (const FInputActionValue& Value)
	{
		UBaseFunctionLibrary::RemovePlayGameTagFromActor(Target, BaseGameplayTags::InputTag_Attack);
		if (APubgPlayerController* Controller = Target->GetController<APubgPlayerController>())
		{
			Controller->RemoveGameplayTagToControlledPawn(BaseGameplayTags::InputTag_Attack);
		}
	});
}
