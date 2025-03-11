// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBluezone.generated.h"

class APubgCharacter;
class UCapsuleComponent;

USTRUCT(BlueprintType)
struct FBlueZoneInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FVector_NetQuantize BlueZoneLocation;

	UPROPERTY()
	float BlueZoneRadius;
};

UCLASS()
class DEDICATED_API ABaseBluezone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseBluezone();
	
protected:
	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* BluezoneComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BluezoneMesh;

	UPROPERTY(ReplicatedUsing=OnRep_BlueZoneInfo)
	FBlueZoneInfo BlueZoneInfo;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;

	void SetBlueZoneInfo(const FVector& NewLocation, float NewRadius);
	
	UFUNCTION()
	void OnRep_BlueZoneInfo();
};
