// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Debug/DebugProjectile.h"

#include "Net/UnrealNetwork.h"


ADebugProjectile::ADebugProjectile() 
{
	bReplicates = true;
	bNetTemporary = false;
	bReplicatePubgMovement = false;
	Super::SetReplicateMovement(false);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	NetPriority = 2.f;
	MinNetUpdateFrequency = 100.0f;
}

void ADebugProjectile::OnRep_ProjReplicatedMovement()
{
	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		//ReplicatedAccel = UTReplicatedMovement.Acceleration;
		FRepMovement& RepMovement = GetReplicatedMovement_Mutable();
		RepMovement.Location = ProjReplicatedMovement.Location;
		RepMovement.Rotation = ProjReplicatedMovement.Rotation;
		RepMovement.LinearVelocity = ProjReplicatedMovement.LinearVelocity;
		RepMovement.AngularVelocity = FVector(0.f);
		RepMovement.bSimulatedPhysicSleep = false;
		RepMovement.bRepPhysics = false;

		OnRep_ReplicatedMovement();
	}
}

void ADebugProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (GetNetMode() == NM_Client || GIsEditor)
	{
		// DrawDebugSphere(GetWorld(), GetActorLocation(), 5.f, 20, FColor::Red);
		
		FVector CurrentLocation = GetActorLocation();
		DrawDebugSphere(GetWorld(), CurrentLocation, 5.f, 20, FColor::Red);
		// 진행 경로 라인 디버깅
		if (!LastLocation.IsZero()) // 이전 위치가 유효하면
		{
			DrawDebugLine(GetWorld(), LastLocation, CurrentLocation, FColor::Blue, false, 5.0f, 0, 1.5f);
		}
		LastLocation = CurrentLocation;
	}
}

void ADebugProjectile::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass());
	if (BPClass != NULL)
	{
		BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}

	DOREPLIFETIME_CONDITION(ADebugProjectile, ProjReplicatedMovement, COND_SimulatedOrPhysics);

}
