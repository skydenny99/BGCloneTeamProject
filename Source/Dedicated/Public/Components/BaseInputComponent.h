// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "DataAssets/DataAsset_InputConfig.h"
#include "DataAssets/Input/DataAsset_PubgInputConfig.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "GameFramework/PubgPlayerController.h"
#include "BaseInputComponent.generated.h"

class UInputConfig_DataAsset;
/**
 * 
 */
UCLASS()
class DEDICATED_API UBaseInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

	
public:
	template<class UserObject, typename Func>
	void NativeBindAction(UDataAsset_InputConfig* InputConfig, FGameplayTag InputTag, ETriggerType TriggerType, UserObject* CallbackTarget, Func Callback);
	
	template<class UserObject, typename Func>
	void BindAbilityAction(UDataAsset_PubgInputConfig* InputConfig, UserObject* CallbackTarget, Func Callback);

	void BindAttackAction(UDataAsset_PubgInputConfig* InputConfig, ACharacter* Target);

	/*
	기존에 있는 비슷한 NativeBindAction은 인자값중 ETriggerType으로 되어있어 EtriggerEvent가 필요해 오버로딩으로 할순 있지만 그냥 새로 만듬
	 */
	template<class UserObject, typename CallBackFunc>
	void BindNativeInputAction(const UDataAsset_PubgInputConfig* InInputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallBackFunc Func);
	
	template<class UserObject, typename CallBackFunc>
	void BindNativeInputActionInstance(const UDataAsset_PubgInputConfig* InInputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallBackFunc Func);

};

template <class UserObject, typename Func>
void UBaseInputComponent::NativeBindAction(UDataAsset_InputConfig* InputConfig, FGameplayTag InputTag, ETriggerType TriggerType, UserObject* CallbackTarget,
	Func Callback)
{
	check(InputConfig);
	if (UInputAction* InputAction = InputConfig->GetInputActionByTag(InputTag))
	{
		BindAction(InputAction, TriggerType, CallbackTarget, Callback);
	}
}

template <class UserObject, typename Func>
void UBaseInputComponent::BindAbilityAction(UDataAsset_PubgInputConfig* InputConfig, UserObject* CallbackTarget, Func Callback)
{
	check(InputConfig);
	for (auto& [InputTag, InputAction] : InputConfig->AbilityInputConfigs)
	{
		BindAction(InputAction, ETriggerEvent::Triggered, CallbackTarget, Callback, InputTag);
	}
}



template <class UserObject, typename CallBackFunc>
void UBaseInputComponent::BindNativeInputAction(const UDataAsset_PubgInputConfig* InInputConfig,
	const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallBackFunc Func)
{
	checkf(InInputConfig, TEXT("Input Config Data Asset is Null Can Not Proceed with Binding"));

	if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InputTag))
	{
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}

template <class UserObject, typename CallBackFunc>
void UBaseInputComponent::BindNativeInputActionInstance(const UDataAsset_PubgInputConfig* InInputConfig,
	const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallBackFunc Func)
{
	checkf(InInputConfig, TEXT("Input Config Data Asset is Null Can Not Proceed with Binding"));

	if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InputTag))
	{
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}

