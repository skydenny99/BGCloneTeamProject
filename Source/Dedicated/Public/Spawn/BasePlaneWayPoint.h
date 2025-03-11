// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePlaneWayPoint.generated.h"

class UBillboardComponent;
class USceneComponent;
class UBoxComponent;
class UArrowComponent;

UCLASS()
class DEDICATED_API ABasePlaneWayPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePlaneWayPoint();
private:
	// 컴포넌트
	UPROPERTY(VisibleAnywhere)
	UBillboardComponent* Billboard;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = C130Component, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* Arrow;
public:
	FORCEINLINE UBoxComponent* GetCollisionBox() const { return CollisionBox; }
};


