// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BaseType/BaseEnumType.h"
#include "GameFramework/Actor.h"
#include "PubgInventoryCharacter.generated.h"

class APubgCharacter;
class ABaseGear;
class APubgBaseCharacter;
class ABaseGun;
class UInventoryCharacterAnimInstance;
class USkeletalMeshComponent;
class USceneCaptureComponent2D;

UCLASS()
class DEDICATED_API APubgInventoryCharacter : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APubgInventoryCharacter();

	UInventoryCharacterAnimInstance* GetAnimInstance();

	void SetPoses(EPlayerActionMode NewPoses);

 	void SetPrimaryWeapon(const int32 _ItemID);
 	void SetSecondaryWeapon(const int32 _ItemID);
	void SetCurrentWeapon(const FGameplayTag& _WeaponTag);

	USkeletalMesh* GetWeapon(const int32 _Item);
	
	FORCEINLINE void SetOpenInventory(const bool Bool) {bOpenInventory = Bool;}
	FORCEINLINE bool GetOpenInventory() {return bOpenInventory;}
	FORCEINLINE UTextureRenderTarget2D* GetRenderTarget() const {return UniqueRenderTarget;}
	FORCEINLINE void SetOwningPawn(APubgCharacter* _OwningPawn) {OwnerPawn = _OwningPawn;}

	void UpdateGear();

protected:
	
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	APubgCharacter* OwnerPawn;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* SceneCaptureComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UInventoryCharacterAnimInstance* CharacterAnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UTextureRenderTarget2D* UniqueRenderTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	bool bOpenInventory;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* PrimaryMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SecondaryMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* CurrentWeaponMesh;

	UPROPERTY(editanywhere, BlueprintReadWrite, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TMap<int32, USkeletalMesh*> WeaponArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Costume", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Costume", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* LegMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Costume", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FeetMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Costume", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* HelmetMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Costume", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* BackpackMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Costume", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* VestMesh;
};