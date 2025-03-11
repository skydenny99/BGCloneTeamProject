// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEExec/GEExec_DamageCalc.h"

#include "BaseDebugHelper.h"
#include "AbilitySystem/PubgGameplayEffectContext.h"
#include "AbilitySystem/AttributeSet/PlayerAttributeSet.h"

struct PubgDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defence)
	DECLARE_ATTRIBUTE_CAPTUREDEF(HeadDefence)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)
	
	PubgDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, DamageTaken, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, Defence, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, HeadDefence, Target, false);
	}
};

static const PubgDamageStatics& DamageStatics()
{
	static PubgDamageStatics DamageStatics;
	return DamageStatics;
}

UGEExec_DamageCalc::UGEExec_DamageCalc()
{
	RelevantAttributesToCapture.Add(DamageStatics().DefenceDef);
}

void UGEExec_DamageCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FPubgGameplayEffectContext* ContextHandle = static_cast<FPubgGameplayEffectContext*>(Spec.GetContext().Get());
	FGunDamageSpecInfo GunDamageSpecInfo = ContextHandle->GetGunDamageSpecInfo();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float Armor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenceDef, EvaluateParams, Armor);
	Armor = FMath::Max<float>(Armor, 0.f);
	
	float Helemt = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().HeadDefenceDef, EvaluateParams, Helemt);
	Helemt = FMath::Max<float>(Helemt, 0.f);

	float Damage = GunDamageSpecInfo.BaseDamage;
	// Damage *= GunDamageSpecInfo.FireRangeDamageMultiplier;
	
		
	const FHitResult* Hit = Spec.GetContext().GetHitResult();
	if (Hit)
	{
		Debug::Print(FString::Printf(TEXT("Hit Bone: %s"), *Hit->BoneName.ToString()));
		Damage *= CalculateBodyPartDamageMultiplier(Hit->BoneName);
		Damage *= CalculateWeaponMultiplier(GunDamageSpecInfo.GunType, Hit->BoneName);
		Damage *= CalculateArmorMultiplier(Hit->BoneName, Armor, Helemt);
	}
	
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			DamageStatics().DamageTakenProperty,
			EGameplayModOp::Additive,
			Damage
			));
}

float UGEExec_DamageCalc::CalculateBodyPartDamageMultiplier(const FName& BoneName) const
{
	float Multiplier = 1.f;
	if (BoneName == FName("head"))
	{
		Multiplier = 1;
	}
	else if (BoneName == FName("pelvis") || BoneName == FName("spine_01"))
	{
		Multiplier = 0.9f;
	}
	else if (BoneName ==  FName("spine_03"))
	{
		Multiplier = 1.1f;
	}
	else if (BoneName == FName("thigh_l") || BoneName == FName("thigh_r"))
	{
		Multiplier = 0.6f;
	}
	else if (BoneName == FName("calf_l") || BoneName == FName("calf_r"))
	{
		Multiplier = 0.5f;
	}
	else if (BoneName == FName("upperarm_l") || BoneName == FName("upperarm_r"))
	{
		Multiplier = 0.6f;
	}
	else if (BoneName == FName("hand_l") || BoneName == FName("hand_r"))
	{
		Multiplier = 0.3f;
	}
	return Multiplier;
}

float UGEExec_DamageCalc::CalculateWeaponMultiplier(const EGunType& GunType, const FName& BoneName) const
{
	float Multiplier = 1.f;
	int32 BodyType = 0; // 기본 몸통 : 몸통 0, 팔다리 1, 머리 2
	if (BoneName == FName("head"))
	{
		BodyType = 2;
	}
	else if (BoneName == FName("pelvis") || BoneName == FName("spine_01") || BoneName == FName("spine_03"))
	{
		BodyType = 0;
	}
	else
	{
		BodyType = 1;
	}

	float HeadMultiplier = 1.f;
	float BodyMultiplier = 1.f;
	float ArmLegMultiplier = 1.f;
	switch (GunType)
	{
	case EGunType::AR:
		HeadMultiplier = 2.35f;
		BodyMultiplier = 1.f;
		ArmLegMultiplier = 0.9f;
		break;
	case EGunType::LMG:
		HeadMultiplier = 2.35f;
		BodyMultiplier = 1.05f;
		ArmLegMultiplier = 0.9f;
		break;
	case EGunType::DMR:
		HeadMultiplier = 2.35f;
		BodyMultiplier = 1.05f;
		ArmLegMultiplier = 0.95f;
		break;
	case EGunType::SR:
		HeadMultiplier = 2.5f;
		BodyMultiplier = 1.3f;
		ArmLegMultiplier = 0.9f;
		break;
	case EGunType::SG:
		HeadMultiplier = 1.5f;
		BodyMultiplier = 0.9f;
		ArmLegMultiplier = 1.2f;
		break;
	case EGunType::SMG:
		HeadMultiplier = 2.1f;
		BodyMultiplier = 1.05f;
		ArmLegMultiplier = 1.3f;
		break;
	case EGunType::Pistol:
		HeadMultiplier = 2.1f;
		BodyMultiplier = 1.f;
		ArmLegMultiplier = 1.05f;
		break;
	}

	switch (BodyType) // 기본 몸통 : 몸통 0, 팔다리 1, 머리 2
	{
		case 1:
			Multiplier = ArmLegMultiplier;
			break;
		case 2:
			Multiplier = HeadMultiplier;
			break;
		case 0:
		default:
			Multiplier = BodyMultiplier;
			break;
	}
	return Multiplier;
}

float UGEExec_DamageCalc::CalculateArmorMultiplier(const FName& BoneName, float Armor, float Helmet) const
{
	
	if (BoneName == FName("head"))
	{
		return 1.f - Helmet / 100.f;
	}

	if (BoneName == FName("pelvis") || BoneName == FName("spine_01") || BoneName ==  FName("spine_03"))
	{
		return 1.f - Armor / 100.f;
	}
	return 1.f;
}
