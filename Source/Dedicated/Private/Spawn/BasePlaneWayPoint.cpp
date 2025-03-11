// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawn/BasePlaneWayPoint.h"

#include "Components/ArrowComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ABasePlaneWayPoint::ABasePlaneWayPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 컴포넌트 설정 
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	DefaultSceneRoot->SetMobility(EComponentMobility::Static);
	RootComponent = DefaultSceneRoot;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(DefaultSceneRoot);
	Billboard->SetMobility(EComponentMobility::Static);
	Billboard->SetWorldScale3D(FVector(16.0f, 16.0f, 16.0f)); // Set scale to 16.0

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Billboard);
	Arrow->SetMobility(EComponentMobility::Static);
	Arrow->SetWorldRotation(FRotator(-90.f, 0, 0));
	Arrow->SetWorldScale3D(FVector(140.0f, 1.0f, 1.0f));
	
	// 콜리전 박스 추가
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetMobility(EComponentMobility::Static);
	CollisionBox->SetBoxExtent(FVector(2000.f, 2000.f, 600.f)); // 박스 크기 설정
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic")); // 기본 콜리전 설정
}



