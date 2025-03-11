// Fill out your copyright notice in the Description page of Project Settings.


#include "Gear/BaseGear.h"

#include "BaseDebugHelper.h"
#include "Characters/PubgBaseCharacter.h"


// Sets default values
ABaseGear::ABaseGear()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	GearMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GearMesh"));
	GearMesh->SetCollisionProfileName("NoCollision");
	RootComponent = GearMesh;
	
	bReplicates = true;
}

void ABaseGear::OnEquip()
{
	//Debug::Print(FString::Printf(TEXT("OnEquip : %s"), *GetActorLabel()), FColor::Blue);
}

void ABaseGear::OnUnEquip()
{
	//Debug::Print(FString::Printf(TEXT("OnUnEquip : %s"), *GetActorLabel()), FColor::Red);
}

void ABaseGear::OnRep_AttachmentReplication()
{
	Super::OnRep_AttachmentReplication();
	if (IsValid(AttachmentReplication.AttachParent.Get()))
	{
		if (APubgBaseCharacter* GearOwner = Cast<APubgBaseCharacter>(AttachmentReplication.AttachParent.Get()))
		{
			GearMesh->SetLeaderPoseComponent(GearOwner->GetMesh());
		}
	}
}
