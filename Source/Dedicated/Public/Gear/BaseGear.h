// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "BaseGear.generated.h"


UCLASS()
class DEDICATED_API ABaseGear : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* GearMesh;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GearGameplayEffectClass;

	UPROPERTY(EditDefaultsOnly)
	float GearEffectLevel = 1.f;

	UPROPERTY(EditDefaultsOnly, meta=(Categories="Item.Gear"))
	FGameplayTag GearType;
	
	//UI용 추가함. 02.11
	UPROPERTY(EditDefaultsOnly)
	int32 GearItemID;

	float CurrentDurability;
	float MaxDurability;
	
public:	
	// Sets default values for this actor's properties
	ABaseGear();

	TSubclassOf<UGameplayEffect> GetGearGameplayEffectClass() const {return GearGameplayEffectClass;}
	FGameplayTag GetGearType() const {return GearType;}
	float GetGearLevel() const {return GearEffectLevel;}
	int32 GetGearItemID() const {return GearItemID;}

	virtual void OnEquip();
	virtual void OnUnEquip();

	FORCEINLINE USkeletalMeshComponent* GetGearMesh() const {return GearMesh;}

	virtual void OnRep_AttachmentReplication() override;
	
	void SetDurability(float InDurability, float InMaxDurability) { CurrentDurability = InDurability; MaxDurability = InMaxDurability; }
	FORCEINLINE float GetCurrentDurability() const { return CurrentDurability; }
	FORCEINLINE float GetMaxDurability() const { return MaxDurability; }
};
