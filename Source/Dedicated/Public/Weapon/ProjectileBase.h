// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGun.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UProjectilePoolComponent;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class DEDICATED_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:

	FGunDamageSpecInfo DamageSpecInfo;
	UPROPERTY()
	TWeakObjectPtr<APubgBaseCharacter> Shooter;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY()
	UProjectilePoolComponent* OwnerPool;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USphereComponent* CollisionComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovement;
	void InitPool(UProjectilePoolComponent* InOwnerPool) {  OwnerPool = InOwnerPool; }
	
	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	void ActivateProjectile(bool InActive);

	UFUNCTION(BlueprintCallable)
	void InitProjectile(APubgBaseCharacter* InShooter, const FGunSpec& Spec);

	
};
