// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/DataAsset_InputConfig.h"


UInputAction* UDataAsset_InputConfig::GetInputActionByTag(const FGameplayTag& InInputTag)
{
	for (auto& [InputTag, InputAction] : NativeInputConfigs)
	{
		if (InputTag.MatchesTagExact(InInputTag))
		{
			return InputAction;
		}
	}
	return nullptr;
}
