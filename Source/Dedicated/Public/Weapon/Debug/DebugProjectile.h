// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ProjectileBase.h"
#include "DebugProjectile.generated.h"

USTRUCT()
struct FRepPubgProjMovement
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FVector_NetQuantize LinearVelocity;

	UPROPERTY()
	FVector_NetQuantize Location;

	UPROPERTY()
	FRotator Rotation;

	FRepPubgProjMovement()
		: LinearVelocity(ForceInit)
		, Location(ForceInit)
		, Rotation(ForceInit)
	{}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		bOutSuccess = true;

		bool bOutSuccessLocal = true;

		// update location, linear velocity
		Location.NetSerialize(Ar, Map, bOutSuccessLocal);
		bOutSuccess &= bOutSuccessLocal;
		Rotation.SerializeCompressed(Ar);
		LinearVelocity.NetSerialize(Ar, Map, bOutSuccessLocal);
		bOutSuccess &= bOutSuccessLocal;

		return true;
	}

	bool operator==(const FRepPubgProjMovement& Other) const
	{
		if (LinearVelocity != Other.LinearVelocity)
		{
			return false;
		}

		if (Location != Other.Location)
		{
			return false;
		}

		if (Rotation != Other.Rotation)
		{
			return false;
		}
		return true;
	}

	bool operator!=(const FRepPubgProjMovement& Other) const
	{
		return !(*this == Other);
	}
};

/**
 * 
 */
UCLASS()
class DEDICATED_API ADebugProjectile : public AProjectileBase
{
	GENERATED_BODY()
public:
	ADebugProjectile();

	
protected:
	/** Used for replication of our RootComponent's position and velocity */
	UPROPERTY(ReplicatedUsing = OnRep_ProjReplicatedMovement)
	struct FRepPubgProjMovement ProjReplicatedMovement;
	
	UPROPERTY(EditAnywhere)
	bool bReplicatePubgMovement;
	
	UPROPERTY()
	FVector LastLocation = FVector::ZeroVector;
	
	UFUNCTION()
	virtual void OnRep_ProjReplicatedMovement();

	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
