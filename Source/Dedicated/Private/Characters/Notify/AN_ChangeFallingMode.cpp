// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Notify/AN_ChangeFallingMode.h"

#include "AnimInstances/PubgAnimInstance.h"
#include "Characters/PubgCharacter.h"


void UAN_ChangeFallingMode::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                   const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);


	if (!MeshComp) return;

	if (APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(MeshComp->GetOwner()))
	{
		if (UPubgAnimInstance* PlayerAnimInstance = Cast<UPubgAnimInstance>(PlayerCharacter->GetMesh()->GetAnimInstance()))
		{
			PlayerAnimInstance->SetFallingType(EFallingType::Normal);
			//PlayerCharacter->PubgNotTurn();
		}
	}
}
