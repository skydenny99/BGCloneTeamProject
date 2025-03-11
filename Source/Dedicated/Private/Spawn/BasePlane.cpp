// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawn/BasePlane.h"
#include "Spawn/BasePlaneSpawnPoint.h"

#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "Characters/PubgCharacter.h"

#include "BaseDebugHelper.h"
#include "Components/BoxComponent.h"
#include "Spawn/BasePlaneWayPoint.h"

ABasePlane::ABasePlane()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	// RootComponent Set
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	DefaultSceneRoot->SetMobility(EComponentMobility::Movable);
	RootComponent = DefaultSceneRoot;
	
	// CollisionBox Set
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(DefaultSceneRoot);
	CollisionBox->SetBoxExtent(FVector(500.f, 500.f, 200.f)); // 박스 크기 설정
	CollisionBox->SetCollisionProfileName(TEXT("BlockAll")); // 기본 콜리전 설정
	
	// Plane Mesh Set
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(
		TEXT("/Game/Asset/Vehicles/Aircraft/Air.Air"));
	if (MeshAsset.Succeeded())
	{
		//SkeletalMesh()->SetSkeletalMesh(MeshAsset.Object);
		SkeletalMesh=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("C130SKM"));
		SkeletalMesh->SetupAttachment(GetRootComponent());
		SkeletalMesh->SetSkeletalMesh(MeshAsset.Object);
		SkeletalMesh->SetRelativeLocation(FVector(0.f, 0.f, -240.0f));
		SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	}
	else
	{
		DebugText = FString::Printf(TEXT("Error!! _ NoSkeletalMesh"));
		Debug::Print(DebugText,FColor::Red);
	}
	// Initialize default components
	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	//Initialize variables
	DestinationLocation = FVector::ZeroVector;
	SpawnWayPoint = nullptr;
	ValidWhileLoop = false;

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveAsset(TEXT("/Game/_BP/C130Plane/CF_C130FloatCurve.CF_C130FloatCurve"));
	if (CurveAsset.Succeeded())
	{
		C130FloatCurve = CurveAsset.Object;
	}
	else 
	{
		DebugText = FString::Printf(TEXT("Error!! _ NoTimeLineCurve"));
		Debug::Print(DebugText,FColor::Red);
	}
	bReplicates = true;
}

// Called when the game starts or when spawned
void ABasePlane::BeginPlay()
{
	Super::BeginPlay();

	//PubgCharacter 배열에 배치된 모든 Player 감지 후 Add
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APubgCharacter::StaticClass(), PubgCharacter);
	if (Timeline && C130FloatCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, FName("TimelineUpdate"));
		TimelineFinishedCallback.BindUFunction(this, FName("OnTimelineFinished"));
		
		Timeline->AddInterpFloat(C130FloatCurve, TimelineCallback);
		Timeline->SetTimelineFinishedFunc(TimelineFinishedCallback);
		Timeline->SetLooping(false);
	}
	for (AActor* Actor : PubgCharacter)
	{
		// Player 캐스팅
		if (APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(Actor))
		{
			Debug::Print("PlaneInputContext Change",FColor::Yellow);
			PlayerCharacter->Client_PlaneInputContext(true);
		}
	}
	//웨이포인트 배열 초기화 
	ResetWayPoint();
	//웨이포인트 랜덤 
	RandomWayPoint();
	//스폰 포인트 / 웨이포인트 설정  
	SpawnWayPointSet();
	//비행기 출발 
	PlaneFly();
}

// Called to bind functionality to input
void ABasePlane::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// 함수 정의
void ABasePlane::ResetWayPoint()
{
	// 월드 웨이포인트 초기화 
	WorldWayPoint.Empty();

	// 월드에 뿌려져 있는 WayPoint 를 임시 AActor 배열에 넣어서 불러옴  
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasePlaneSpawnPoint::StaticClass(), FoundActors);

	// WayPoint 를 사용하기 위해 AActor* 로 불러온 WayPoint를
	// ABasePlaneWayPoint로 케스팅 해서 만들어둔 WorldWayPoint 배열에 Add
	for (AActor* Actor : FoundActors)
	{
		if (ABasePlaneSpawnPoint* WayPoint = Cast<ABasePlaneSpawnPoint>(Actor))
		{
			WorldWayPoint.Add(WayPoint);
		}
	}
	// 월드에서 발견한 총 웨이포인트 디버깅 
	if (WorldWayPoint.Num() <= 0)
	{
		Debug::Print("Error!! _ Can.t Find WayPoint or No WayPoint in World",FColor::Red);
	}
}

void ABasePlane::RandomWayPoint()
{
	if (WorldWayPoint.Num() > 0)
	{
		// 랜덤 인덱스 생성 웨이포인트 수만큼 랜덤 
		const int32 RandomIndex = FMath::RandRange(0, WorldWayPoint.Num() - 1);
		
		// SpawnWayPoint에 랜덤 선택된 웨티포인트 대입 
		SpawnWayPoint = WorldWayPoint[RandomIndex];
	}
	else
	{
		DebugText = TEXT("Error!! _ Can.t Find WayPoint or No WayPoint in World.");
		Debug::Print(DebugText, FColor::Red);
	}
}

void ABasePlane::SpawnWayPointSet()
{
	ValidWhileLoop = true;
	// 랜덤 선택된 웨이 검증 
	while (ValidWhileLoop)
	{
		if (SpawnWayPoint && SpawnWayPoint->IsValidLowLevel())
		{
			// 비행기 종료 / 점프시작 / 점프종료점 설정
			if (SpawnWayPoint->GetDestinationPoint())
			{
				DestinationLocation = SpawnWayPoint->GetDestinationPoint()->GetActorLocation();
			}
			else
			{
				Debug::Print("Error!!! = SpawnWayPoint->GetDestinationPoint() NULL",FColor::Red);
			}
			//낙하 시작 지점 바인딩 
			if (SpawnWayPoint->GetExitStartPoint())
			{
				SpawnWayPoint->GetExitStartPoint()->GetCollisionBox()->OnComponentBeginOverlap.AddDynamic(this, &ABasePlane::OnOverlapExitStart);
			}
			else
			{
				Debug::Print("Error!!! = GetExitStartPoint()->GetCollisionBox() NULL",FColor::Red);
			}

			//낙하 종료 지점 바인딩 
			if (SpawnWayPoint->GetExitEndPoint())
			{
				SpawnWayPoint->GetExitEndPoint()->GetCollisionBox()->OnComponentBeginOverlap.AddDynamic(this, &ABasePlane::OnOverlapExitEnd);
			}
			else
			{
				Debug::Print("Error!!! = GetExitEndPoint()->GetCollisionBox() NULL",FColor::Red);
			}
			// 반복문 정지 
			ValidWhileLoop = false;

			// 비행기 시작 지점으로 이동 
			SetActorLocation(SpawnWayPoint->GetActorLocation(), false, nullptr, ETeleportType::None);

			// 비행기를 웨이포인트 에로우 방향으로 전환 
			SetActorRotation(SpawnWayPoint->GetArrowComponent()->GetComponentRotation(), ETeleportType::None);
		}
		else
		{
			// 만약 스폰포인트가 유효 하지 않다면 
			Debug::Print("Error: WayPoint Is Not Valid - Random", FColor::Red);
			RandomWayPoint();
			// 다시 랜덤을 돌리고 반복
		}
	}
	bHasExitedStart = false;
	bHasExitedEnd=false;
	bHasReachedDestination = false;
}
	
void ABasePlane::TimelineUpdate(const float Value)
{
	// 이동 위치값 적용
	const FVector NewLocation = FMath::VInterpTo(GetActorLocation(), DestinationLocation, Value ,PlaneSpeed);
	Multicast_UpdatePlaneLocation(NewLocation);
	
	// DestinationLocation 조건 체크
	if (!bHasReachedDestination && FVector::Dist(GetActorLocation(), DestinationLocation) <= 100.0f)
	{
		bHasReachedDestination = true; // 이미 처리되었음을 기록
		// 타임라인 정지 및 비행기 제거
		Timeline->Stop();
		OnTimelineFinished();
	}
}
void ABasePlane::Multicast_UpdatePlaneLocation_Implementation(FVector NewLocation)
{
	SetActorLocation(NewLocation, true, nullptr, ETeleportType::None);
}
void ABasePlane::OnTimelineFinished()
{
	PlaneDestroy();
}

bool ABasePlane::PlaneFly_Validate()
{
	return true;
}

void ABasePlane::PlaneFly_Implementation()
{ 
	// 비행 로직 추가
	if (Timeline && !Timeline->IsPlaying())
	{
		Timeline->PlayFromStart();
	}
}

bool ABasePlane::PlaneDestroy_Validate()
{
	return true;
}

void ABasePlane::PlaneDestroy_Implementation()
{
	Destroy();
}

bool ABasePlane::OnOverlapExitStart_Validate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
										 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	return true;
}

void ABasePlane::OnOverlapExitStart_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Debug::Print(TEXT("Start"),FColor::Red);
	// ExitStartLocation 조건 체크
	if (!bHasExitedStart)
	{
		bHasExitedStart = true; // 이미 처리되었음을 기록
		
		for (AActor* Actor : PubgCharacter)
		{
			// Player 캐스팅
			if (APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(Actor))
			{
				PlayerCharacter->SetCanExit(true);
			}
		}
	}
}

bool ABasePlane::OnOverlapExitEnd_Validate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	return true;      
}

void ABasePlane::OnOverlapExitEnd_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Debug::Print(TEXT("End"),FColor::Red);
	// ExitEndLocation 조건 체크
	if (!bHasExitedEnd)
	{
		bHasExitedEnd = true; // 이미 처리되었음을 기록
		
		for (AActor* Actor : PubgCharacter)
		{
			// Ture인 유저만 드랍 
			if (APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(Actor))
			{
				PlayerCharacter->PlaneExitPlayer();
			}
		}
	}
}