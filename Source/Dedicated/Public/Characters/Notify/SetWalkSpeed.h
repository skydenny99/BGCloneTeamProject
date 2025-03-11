// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SetWalkSpeed.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API USetWalkSpeed : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed)
	bool bIsFirst;
	
	/**If bIsFirst is false, apply the current mode's movement speed; else, apply 0.5.*/
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
