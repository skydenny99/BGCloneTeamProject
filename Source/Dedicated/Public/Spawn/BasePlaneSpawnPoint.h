// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePlaneSpawnPoint.generated.h"


class UArrowComponent;
class UBillboardComponent;
class ABasePlaneWayPoint;

UCLASS()
class DEDICATED_API ABasePlaneSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePlaneSpawnPoint();
private:
#pragma region Components

	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultSceneRoot;
	UPROPERTY(VisibleAnywhere)
	UArrowComponent* Arrow;
	UPROPERTY(VisibleAnywhere)
	UBillboardComponent* Billboard;

	
#pragma endregion

#pragma region WayPoints,ExitPoints
	// 웨이포인트 Exit 포인트
	
	UPROPERTY(EditAnywhere , Category = "_WayPoints")
	ABasePlaneWayPoint* ExitStartPoint;

	UPROPERTY(EditAnywhere, Category = "_WayPoints")
	ABasePlaneWayPoint* ExitEndPoint;

	UPROPERTY(EditAnywhere, Category = "_WayPoints")
	ABasePlaneWayPoint* DestinationPoint;

#pragma endregion
	
public:
#pragma region Getter
	//웨이포인트 Arrow 방향 겟터 
	FORCEINLINE UArrowComponent* GetArrowComponent() const { return Arrow; }
	//목적지점 겟터
	FORCEINLINE ABasePlaneWayPoint* GetDestinationPoint() const { return DestinationPoint; }
	//점프 시작 지점
	FORCEINLINE ABasePlaneWayPoint* GetExitStartPoint() const { return ExitStartPoint; }
	//점프 마지막 지점 
	FORCEINLINE ABasePlaneWayPoint* GetExitEndPoint() const { return ExitEndPoint; }
#pragma endregion
};
