// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEExec/GEExec_FallDamageCalc.h"

#include "AbilitySystem/AttributeSet/PlayerAttributeSet.h"
#include "Characters/PubgCharacter.h"

struct PubgFallDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)
	
	PubgFallDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, DamageTaken, Target, false);
	}
};

static const PubgFallDamageStatics& FallDamageStatics()
{
	static PubgFallDamageStatics FallDamageStatics;
	return FallDamageStatics;
}

void UGEExec_FallDamageCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	AActor* SourceActor = Spec.GetContext().GetEffectCauser();
	float FallDamage = 0.f;
	if (APubgCharacter* Character = Cast<APubgCharacter>(SourceActor))
	{
		
		const float FallVelocity = Character->GetVelocity().Z;
		// 속도 기반 데미지 (*숫자가 클수록 들어오는 대미지가 커짐 )
		//(FallVelocity/ 1000)*5.1f;
		FallDamage = FMath::Pow(FMath::Abs(FallVelocity) / 1000.0f, 5.1f); 
	}

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			FallDamageStatics().DamageTakenProperty,
			EGameplayModOp::Additive,
			FallDamage
			));
}
