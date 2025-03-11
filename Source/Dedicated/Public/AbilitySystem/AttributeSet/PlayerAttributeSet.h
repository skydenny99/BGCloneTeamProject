// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "PlayerAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class DEDICATED_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UPlayerAttributeSet();

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	
	UPROPERTY(BlueprintReadOnly, Category="Status", ReplicatedUsing=OnRep_CurrentHp)
	FGameplayAttributeData CurrentHp;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, CurrentHp)
	
	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData MaxHp;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MaxHp)

	UPROPERTY(BlueprintReadOnly, Category="Status", ReplicatedUsing=OnRep_CurrentBoost)
	FGameplayAttributeData CurrentBoost;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, CurrentBoost)

	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData MaxBoost;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MaxBoost)

	UPROPERTY(BlueprintReadOnly, Category="Status", ReplicatedUsing=OnRep_Defence)
	FGameplayAttributeData Defence;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Defence)
	
	UPROPERTY(BlueprintReadOnly, Category="Status", ReplicatedUsing=OnRep_HeadDefence)
	FGameplayAttributeData HeadDefence;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, HeadDefence)

	UPROPERTY(BlueprintReadOnly, Category="Status", ReplicatedUsing=OnRep_InventoryCapacity)
	FGameplayAttributeData InventoryCapacity;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, InventoryCapacity)
	
	UPROPERTY(BlueprintReadWrite, Category="Status", ReplicatedUsing=OnRep_MaxWalkSpeed)
	FGameplayAttributeData MaxWalkSpeed;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MaxWalkSpeed)
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void BP_SetMaxWalkSpeed(const float Speed) { SetMaxWalkSpeed(Speed); }
	
	
	UPROPERTY(BlueprintReadOnly, Category="Status")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, DamageTaken)
	
	UFUNCTION()
	virtual void OnRep_CurrentHp(const FGameplayAttributeData& InCurrentHp);
	
	UFUNCTION()
	virtual void OnRep_CurrentBoost(const FGameplayAttributeData& InCurrentBoost);
	
	UFUNCTION()
	virtual void OnRep_Defence(const FGameplayAttributeData& InDefence);
	
	UFUNCTION()
	virtual void OnRep_HeadDefence(const FGameplayAttributeData& InHeadDefence);
	
	UFUNCTION()
	virtual void OnRep_InventoryCapacity(const FGameplayAttributeData& InInventoryCapacity);
	
	UFUNCTION()
	virtual void OnRep_MaxWalkSpeed(const FGameplayAttributeData& InMaxWalkSpeed);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
};

inline void UPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, CurrentHp, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, CurrentBoost, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Defence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, HeadDefence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, InventoryCapacity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, MaxWalkSpeed, COND_None, REPNOTIFY_Always);
}
