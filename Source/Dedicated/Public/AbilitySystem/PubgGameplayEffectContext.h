// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectTypes.h"
#include "BaseType/BaseStructType.h"
#include "PubgGameplayEffectContext.generated.h"


USTRUCT()
struct DEDICATED_API FPubgGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_USTRUCT_BODY()
	
public:
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FPubgGameplayEffectContext::StaticStruct();
	}

	virtual FPubgGameplayEffectContext* Duplicate() const override
	{
		FPubgGameplayEffectContext* NewContext = new FPubgGameplayEffectContext();
		*NewContext = *this;
		NewContext->AddActors(Actors);
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		// Shallow copy of TargetData, is this okay?
		NewContext->GunDamageSpecInfo = GunDamageSpecInfo;
		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	void SetGunDamageSpecInfo(const FGunDamageSpecInfo& SpecInfo) { GunDamageSpecInfo = SpecInfo; }
	const FGunDamageSpecInfo& GetGunDamageSpecInfo() const {return GunDamageSpecInfo;}

protected:
	FGunDamageSpecInfo GunDamageSpecInfo;
};


template<>
struct TStructOpsTypeTraits<FPubgGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FPubgGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};