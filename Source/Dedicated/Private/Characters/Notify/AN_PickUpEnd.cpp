// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Notify/AN_PickUpEnd.h"
#include "Characters/PubgCharacter.h"


void UAN_PickUpEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                           const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(MeshComp->GetOwner()))
	{
		PlayerCharacter->SetPickUp(false);
	}
}
