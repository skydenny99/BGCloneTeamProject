// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Input/DataAsset_PubgInputConfig.h"


UInputAction* UDataAsset_PubgInputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	for (const FBaseInputActionConfig& InputActionConfig : NativeInputActions)
	{
		if (InputActionConfig.InputTag == InInputTag && InputActionConfig.InputAction)
		{
			return InputActionConfig.InputAction;
		}
	}
	return nullptr;
}
