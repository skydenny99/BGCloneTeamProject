// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/BaseFieldItem.h"

#include "BaseDebugHelper.h"
#include "Characters/PubgCharacter.h"
#include "Components/BaseInteractComponent.h"
#include "Components/Renewal/PickUpManageComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"


ABaseFieldItem::ABaseFieldItem()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetCollisionProfileName("NoCollision");

	InteractComponent = CreateDefaultSubobject<UBaseInteractComponent>(TEXT("InteractComponent"));

	NetDormancy = DORM_DormantAll;
	
}

void ABaseFieldItem::BeginPlay()
{
	Super::BeginPlay();
	if (bisWorldSpawn)
	{
		TArray<AActor*> OverlappingActors;
		GetOverlappingActors(OverlappingActors,APubgCharacter::StaticClass());
		if (OverlappingActors.Num() > 0)
		{
			for (AActor* _PlayerCharacterPtr : OverlappingActors)
			{
				if (APubgCharacter* _PlayerCharacter = Cast<APubgCharacter>(_PlayerCharacterPtr))
				{
					if (UPickUpManageComponent* PickUpComponent = _PlayerCharacter->GetPickUpManageComponent())
					{
						PickUpComponent->AddPickUpCandidate(this);
						_PlayerCharacter->UpdateInventory();
					}
				}
			}
		}
	}
}

void ABaseFieldItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseFieldItem, ItemAmount);
	DOREPLIFETIME(ABaseFieldItem, bisWorldSpawn);
}

void ABaseFieldItem::SnapOnGround()
{
	const FVector EndLocation = GetActorLocation() + FVector::DownVector * 500.f;
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	DrawDebugLine(GetWorld(), GetActorLocation(), EndLocation, FColor::Red, true);
	if (GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), EndLocation, ECC_GameTraceChannel2))
	{
		SetActorLocation(Hit.Location);
		SetActorRotation(UKismetMathLibrary::MakeRotFromZX(Hit.Normal, GetActorForwardVector()));
	}
}


