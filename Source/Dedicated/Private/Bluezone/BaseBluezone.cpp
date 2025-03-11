// Fill out your copyright notice in the Description page of Project Settings.


#include "Bluezone/BaseBluezone.h"

#include "BaseGameplayTags.h"
#include "Characters/PubgCharacter.h"
#include "Components/CapsuleComponent.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "GameFramework/PubgPlayerController.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ABaseBluezone::ABaseBluezone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BluezoneComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BluezoneComponent"));
	BluezoneComponent->InitCapsuleSize(100.f, 500000.f);
	BluezoneComponent->SetCollisionProfileName("Bluezone");
	BluezoneComponent->SetHiddenInGame(false);
	RootComponent = BluezoneComponent;

	BluezoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BluezoneMesh->SetupAttachment(RootComponent);
	
	
	bReplicates = true;
}

void ABaseBluezone::BeginPlay()
{
	Super::BeginPlay();
	if (GetNetMode() == NM_Client)
	{
		BluezoneComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseBluezone::OnCharacterBeginOverlap);
		BluezoneComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseBluezone::OnCharacterEndOverlap);
	}
}

void ABaseBluezone::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (APubgCharacter* Character = Cast<APubgCharacter>(OtherActor))
	{
		if (Character->IsLocallyControlled())
		{
			if (APubgPlayerController* Controller = Character->GetController<APubgPlayerController>())
			{
				Controller->RemoveGameplayTagToControlledPawn(BaseGameplayTags::State_Debuff_BluezoneDamage);
			}
			
			if (Character->GetWorld()->PostProcessVolumes.Num() > 0)
			{
				FPostProcessVolumeProperties Volume = Character->GetWorld()->PostProcessVolumes[0]->GetProperties();
				FPostProcessSettings* Settings = const_cast<FPostProcessSettings*>(Volume.Settings);
				Settings->ColorGain = FVector4::One();
			}
		}
	}
}

void ABaseBluezone::OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APubgCharacter* Character = Cast<APubgCharacter>(OtherActor))
	{
		if (Character->IsLocallyControlled())
		{
			if (APubgPlayerController* Controller = Character->GetController<APubgPlayerController>())
			{
				Controller->AddGameplayTagToControlledPawn(BaseGameplayTags::State_Debuff_BluezoneDamage);
			}

			if (Character->GetWorld()->PostProcessVolumes.Num() > 0)
			{
				FPostProcessVolumeProperties Volume = Character->GetWorld()->PostProcessVolumes[0]->GetProperties();
				FPostProcessSettings* Settings = const_cast<FPostProcessSettings*>(Volume.Settings);
				Settings->ColorGain = FVector4(0.6f, 0.6f, 1.f, 1.f);
			}
		}
	}
}

void ABaseBluezone::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseBluezone, BlueZoneInfo);
}

bool ABaseBluezone::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget,
	const FVector& SrcLocation) const
{
	return true; //Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
}

void ABaseBluezone::SetBlueZoneInfo(const FVector& NewLocation, float NewRadius)
{
	BlueZoneInfo.BlueZoneLocation = NewLocation;
	BlueZoneInfo.BlueZoneRadius = NewRadius;
	OnRep_BlueZoneInfo();
}

void ABaseBluezone::OnRep_BlueZoneInfo()
{
	BluezoneComponent->SetCapsuleRadius(BlueZoneInfo.BlueZoneRadius, true);
	BluezoneMesh->SetRelativeScale3D(FVector(BlueZoneInfo.BlueZoneRadius / 50.f, BlueZoneInfo.BlueZoneRadius / 50.f, 50000.f));
	SetActorLocation(BlueZoneInfo.BlueZoneLocation);
}
