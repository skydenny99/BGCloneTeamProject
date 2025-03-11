// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_ActiveToggleMovement.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API UAN_ActiveToggleMovement : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDisable;

	// Disable이 True면 DisableMovement False면 SetMovement(Walk 모드로)
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
