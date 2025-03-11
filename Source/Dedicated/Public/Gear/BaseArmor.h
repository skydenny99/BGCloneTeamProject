// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gear/BaseGear.h"
#include "BaseArmor.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATED_API ABaseArmor : public ABaseGear
{
	GENERATED_BODY()
	
	
public:
	virtual void OnEquip() override;
	
};
