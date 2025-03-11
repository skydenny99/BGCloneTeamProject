// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_ReloadAmmo_SingleLoad.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API UAN_ReloadAmmo_SingleLoad : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	int32 ReloadAmmoAmount = 1;

	UPROPERTY(EditAnywhere)
	FString ReloadEndSectionName;
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
