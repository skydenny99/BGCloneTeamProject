// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Notify/AN_ActiveToggleMovement.h"

#include "Characters/PubgCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


void UAN_ActiveToggleMovement::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	if (APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(MeshComp->GetOwner()))
	{
		if (UCharacterMovementComponent* CharacterMovementComp = PlayerCharacter->GetCharacterMovement())
		{
			if (bDisable)
			{
				CharacterMovementComp->DisableMovement();
				PlayerCharacter->SetIsLanding(true);
			}
			else
			{
				CharacterMovementComp->SetMovementMode(MOVE_Walking);
				if (PlayerCharacter->GetHasTurn())
				{
					PlayerCharacter->SetHasTurn(false);
					PlayerCharacter->bUseControllerRotationYaw = true;
				}
				PlayerCharacter->SetIsLanding(false);
			}
		}
	}
	
}
