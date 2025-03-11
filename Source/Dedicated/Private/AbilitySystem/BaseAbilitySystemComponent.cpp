// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAbilitySystemComponent.h"


UBaseAbilitySystemComponent::UBaseAbilitySystemComponent()
{
	ReplicationMode = EGameplayEffectReplicationMode::Mixed;
}

void UBaseAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag Tag)
{
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability->AbilityTags.HasTagExact(Tag) == false) continue;
		if (AbilitySpec.Handle.IsValid())
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UBaseAbilitySystemComponent::SetActiveGameplayEffectLevelByTag(const FGameplayTag& EffectTag, int32 NewLevel)
{
	if (EffectTag.IsValid() == false || NewLevel == 0) return;
	FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(EffectTag.GetSingleTagContainer());
	TArray<FActiveGameplayEffectHandle> EffectHandles = GetActiveEffects(Query);
	for (auto& EffectHandle : EffectHandles)
	{
		SetActiveGameplayEffectLevel(EffectHandle, NewLevel);
	}
}
