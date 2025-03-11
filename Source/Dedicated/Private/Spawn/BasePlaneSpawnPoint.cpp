// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawn/BasePlaneSpawnPoint.h"
#include "Components/ArrowComponent.h"
#include "Components/BillboardComponent.h"
#include "Spawn/BasePlaneWayPoint.h"


// Sets default values
ABasePlaneSpawnPoint::ABasePlaneSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 컴포넌트 설정 
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	DefaultSceneRoot->SetMobility(EComponentMobility::Static); // 정적 설정
	RootComponent = DefaultSceneRoot;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(DefaultSceneRoot);
	Billboard->SetMobility(EComponentMobility::Static);
	Billboard->SetWorldScale3D(FVector(16.0f, 16.0f, 16.0f)); // Set scale to 16.0

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Billboard);
	Arrow->SetMobility(EComponentMobility::Static);
	Arrow->SetWorldScale3D(FVector(140.0f, 1.0f, 1.0f));

}


