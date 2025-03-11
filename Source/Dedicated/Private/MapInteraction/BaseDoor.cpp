// Fill out your copyright notice in the Description page of Project Settings.


#include "MapInteraction/BaseDoor.h"
#include "Components/TimelineComponent.h"
#include "BaseDebugHelper.h"
#include "Characters/PubgCharacter.h"
#include "Components/BoxComponent.h"


// Sets default values
ABaseDoor::ABaseDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true; //  네트워크 동기화 활성화

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	DefaultSceneRoot->SetMobility(EComponentMobility::Movable);
	RootComponent = DefaultSceneRoot;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorFrameAsset
	(TEXT("/Game/MilitaryAirport/US_Military/Meshes/Buildings/SM_FFloorDoor300x450.SM_FFloorDoor300x450"));
	if (DoorFrameAsset.Succeeded())
	{
		DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
		DoorFrame->SetupAttachment(RootComponent);
		DoorFrame->SetStaticMesh(DoorFrameAsset.Object);
		DoorFrame->SetVisibility(false);
	}
	else
	{
		DebugText = FString::Printf(TEXT("Error!! DoorFrame _ NoStaticMesh"));
		Debug::Print(DebugText,FColor::Red);
	}
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorAsset
	(TEXT("/Game/MilitaryAirport/US_Military/Meshes/Buildings/SM_Door_002.SM_Door_002"));
	if (DoorAsset.Succeeded())
	{
		Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
		Door->SetupAttachment(DoorFrame);
		Door->SetStaticMesh(DoorAsset.Object);
		Door->SetRelativeLocation(FVector(100.f, -8.f, 0.f));
	}
	else
	{
		DebugText = FString::Printf(TEXT("Error!! DoorFrame _ NoStaticMesh"));
		Debug::Print(DebugText,FColor::Red);
	}
	
	// // 콜리전 박스 추가
	// CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	// CollisionBox->SetupAttachment(DoorFrame);
	// CollisionBox->SetRelativeLocation(FVector(150.0f, 0.f, 210.0f));
	// CollisionBox->SetBoxExtent(FVector(200.f, 100.f, 300.f)); // 박스 크기 설정
	// CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic")); // 기본 콜리전 설정
	
	static ConstructorHelpers::FObjectFinder<UCurveFloat> DoorCurve
	(TEXT("/Game/_BP/MapObj/DoorCurveFloat.DoorCurveFloat"));
	if (DoorCurve.Succeeded())
	{
		CurveFloat = DoorCurve.Object;
	}
	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ParachuteTimeline"));

	
}

void ABaseDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseDoor, Character);
}

// Called when the game starts or when spawned
void ABaseDoor::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("OpenDoor"));
		Timeline->AddInterpFloat(CurveFloat, TimelineProgress, FName("OpenDoor"));
	}
	else
	{
		DebugText = FString::Printf(TEXT("Error!! CurveFloat is nullptr"));
		Debug::Print(DebugText, FColor::Red);
	}
}

void ABaseDoor::OnInteract()
//void ABaseDoor::OnInteract_Implementation()
{
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (APawn* OwnerPawn = PlayerController->GetPawn())
		{
			SetOwner(OwnerPawn); // 소유권을 설정
		}
		else
		{
			DebugText = FString::Printf(TEXT("No APawn"));
			Debug::Print(DebugText, FColor::Blue);
		}
	}
	else
	{
		DebugText = FString::Printf(TEXT("No PlayerController"));
		Debug::Print(DebugText, FColor::Red);
	}
	
	if (!bIsDoorClosed)
	{
		SetDoorOnSameSide();
		Timeline->Play(); // Open the Door
		DebugText = FString::Printf(TEXT("Door Open"));
		Debug::Print(DebugText, FColor::Blue);
	}
	else
	{
		Timeline->Reverse(); // Close the Door
		DebugText = FString::Printf(TEXT("Door Close"));
		Debug::Print(DebugText, FColor::Blue);
	}
	bIsDoorClosed = !bIsDoorClosed; // Flip Flop
}

void ABaseDoor::OpenDoor(float Value)
{
	const float Angle = bDoorOnSameSide ?  -DoorRotateAngle : DoorRotateAngle;
	const FRotator Rot = FRotator(0.f, Angle * Value, 0.f);
	if (Door)
	{
		Door->SetRelativeRotation(Rot);
	}
}
void ABaseDoor::SetDoorOnSameSide()
//void ABaseDoor::SetDoorOnSameSide_Implementation()
{
	DebugText = FString::Printf(TEXT("SmaeSideCheak"));
	Debug::Print(DebugText,FColor::Yellow);
	if (Character)
	{
		const FVector CharacterFv = Character->GetActorForwardVector();
		const FVector DoorFv = -GetActorRightVector();
		bDoorOnSameSide = (FVector:: DotProduct(CharacterFv, DoorFv) >= 0.f);
	}
	else
	{
		DebugText = FString::Printf(TEXT("Input_Interacting called on %s"), HasAuthority() ? TEXT("Server") : TEXT("Client"));
		Debug::Print(DebugText,FColor::Red);
		DebugText = FString::Printf(TEXT("No Player Find"));
		Debug::Print(DebugText,FColor::Red);
	}
}
