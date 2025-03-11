// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "BaseFieldItem.generated.h"

class UBaseInteractComponent;

UCLASS()
class DEDICATED_API ABaseFieldItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseFieldItem();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditDefaultsOnly)
	UBaseInteractComponent* InteractComponent;

	UPROPERTY(EditDefaultsOnly, Category="ItemInfo")
	int32 ItemID;
	
	UPROPERTY(EditDefaultsOnly, Category="ItemInfo", meta=(Categories="Item"))
	FGameplayTag ItemTypeTag;

	UPROPERTY(Replicated, EditDefaultsOnly, Category="ItemInfo")
	int32 ItemAmount;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadOnly, Category="ItemInfo", meta = (AllowPrivateAccess = "true"))
	bool bisWorldSpawn = false;

public:
	void SetAmount(int32 Amount)
	{
		ForceNetUpdate();
		ItemAmount = Amount;
	}

	void SnapOnGround();
	
	FORCEINLINE int32 GetItemID() const { return ItemID; }
	FORCEINLINE int32 GetItemAmount() const { return ItemAmount; }
	FORCEINLINE UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }
	FORCEINLINE void SetWorldSpawned(bool _Bool) { bisWorldSpawn = _Bool; }
	FORCEINLINE const FGameplayTag& GetItemType() const { return ItemTypeTag; }
	
};

