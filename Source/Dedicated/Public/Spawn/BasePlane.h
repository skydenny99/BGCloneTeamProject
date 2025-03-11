// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePlane.generated.h"

class UInputComponent;
class USplineComponent;
class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class UTimelineComponent;
class ABasePlaneSpawnPoint;
class UBoxComponent;
class UDataAsset_PubgInputConfig;
UCLASS()
class DEDICATED_API ABasePlane : public APawn //public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region Components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "_C130Component", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "_C130Component", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "_C130Component", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "_C130Component", meta = (AllowPrivateAccess = "true"))
	UTimelineComponent* Timeline;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "_C130Component", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "_C130Component", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* ExitStartTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "_C130Component", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* ExitEndTrigger;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "_C130Component", meta = (AllowPrivateAccess = "true"))
	USceneComponent* DefaultSceneRoot;

#pragma endregion
	
private:
	
#pragma region Variables
	UPROPERTY()
	FVector DestinationLocation;
	
	UPROPERTY()
	TArray<ABasePlaneSpawnPoint*> WorldWayPoint;

	UPROPERTY()
	TArray<AActor*> PubgCharacter;
	
	UPROPERTY(VisibleAnyWhere)
	ABasePlaneSpawnPoint* SpawnWayPoint;
	
	UPROPERTY()
	bool ValidWhileLoop;
	UPROPERTY()
	bool bHasExitedStart = false; // ExitStartLocation 체크 여부
	UPROPERTY()
	bool bHasExitedEnd = false;   // ExitEndLocation 체크 여부
	UPROPERTY()
	bool bHasReachedDestination = false; // DestinationLocation 체크 여부
	
	UPROPERTY(EditAnyWhere , Category = "Config_SpeedSet = Slow 0 ~ 1 Fast")
	float PlaneSpeed = 0.5f;
	
	UPROPERTY()
	FString DebugText;

	UPROPERTY(EditAnywhere)
	UCurveFloat* C130FloatCurve;

#pragma endregion

	
#pragma region Functions
	
	UFUNCTION()
	void ResetWayPoint();
	UFUNCTION()
	void RandomWayPoint();
	UFUNCTION()
	void SpawnWayPointSet();
	
	// Timeline functions
	UFUNCTION()
	void TimelineUpdate(const float Value);

	UFUNCTION()
	void OnTimelineFinished();

	// Server RPC
	UFUNCTION(Server, Reliable, WithValidation)
	void PlaneFly();
	bool PlaneFly_Validate();
	void PlaneFly_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void PlaneDestroy();
	bool PlaneDestroy_Validate();
	void PlaneDestroy_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdatePlaneLocation(FVector NewLocation);
	
	//웨이 포인트 오버랩
	UFUNCTION(Server, Reliable, WithValidation)
	void OnOverlapExitStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
										 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	bool OnOverlapExitStart_Validate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
										 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void OnOverlapExitStart_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
										UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable, WithValidation)
	void OnOverlapExitEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
										 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	bool OnOverlapExitEnd_Validate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
										 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnOverlapExitEnd_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
										 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
#pragma endregion
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

};
