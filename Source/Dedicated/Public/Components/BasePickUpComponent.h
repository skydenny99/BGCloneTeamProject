// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BasePickUpComponent.generated.h"


struct FGameplayTag;
enum class EPickMode : uint8;
class ABaseWeaponFieldItem;
class ABaseFieldItem;
class APubgCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEDICATED_API UBasePickUpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBasePickUpComponent();

protected:
	UPROPERTY()
	TArray<ABaseFieldItem*> PickUpCandidates;

	UPROPERTY()
	APubgCharacter* PubgCharacter;

	virtual void BeginPlay() override;
	
public:
    void AddPickUpCandidate(ABaseFieldItem* PickUpItem);
	void RemovePickUpCandidate(ABaseFieldItem* PickUpItem);

	// Call from client
	void PickUp(ABaseFieldItem* InPickUpItem);

	UFUNCTION(Server, Reliable)
	void Server_PickUp(ABaseFieldItem* InPickUpItem, bool NeedToPlayMontage);

	UFUNCTION(Server, Unreliable)
	void Server_DiscardPickUp(int32 ItemID, int32 ItemAmount);

	UFUNCTION(Client, Reliable)
	void Client_AddPickUpCandidate(ABaseFieldItem* PickUpItem);

	UFUNCTION(Server, Unreliable)
	void Server_DiscardWeapon(FGameplayTag EquipTag);


	UFUNCTION()
	void Debug_ShowPickUpCandidates();

	UFUNCTION()
	void Debug_PickUpCandidateByIndex(int32 InIndex);

	UFUNCTION(Client, Reliable)
	void Client_UpdateWidget();

	UFUNCTION(Client, Reliable)
	void Client_WeaponUpdate();

protected:
	void InternalPickUpItem(ABaseFieldItem* InPickUpItem);
	void InternalPickUpWeapon(ABaseWeaponFieldItem* PickUpItem);
	void InternalPickUpArmor(ABaseFieldItem* PickUpItem);
	EPickMode GetPickMode(const float LocationZ) const;
	
public:
	FORCEINLINE TArray<ABaseFieldItem*> GetPickUpCandidates() { return PickUpCandidates; } 
	
};