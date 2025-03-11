// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartupData/DataAsset_StartupBase.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"

/*
void UDataAsset_StartupBase::GiveToAbilitySystemComponent(UBaseAbilitySystemComponent* ASC, int32 Level)
{
	check(ASC);

	GrantAbilities(ActivateOnGivenAbilities, ASC, Level);
	GrantAbilities(ReactiveAbilities, ASC, Level);
}

void UDataAsset_StartupBase::GrantAbilities(const TArray<TSubclassOf<UBaseGameplayAbility>> GAs,
	UBaseAbilitySystemComponent* InASC, int32 Level)
{
	if (GAs.IsEmpty())
	{
		return;
	}

	for (const TSubclassOf<UBaseGameplayAbility> Ability : GAs)
	{
		FGameplayAbilitySpec Spec(Ability);
		Spec.SourceObject = InASC->GetAvatarActor();
		Spec.Level = Level;

		InASC->GiveAbility(Spec);
	}
}
*/