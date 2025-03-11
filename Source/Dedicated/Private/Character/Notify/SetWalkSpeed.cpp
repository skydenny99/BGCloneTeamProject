// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Notify/SetWalkSpeed.h"
#include "Characters/PubgCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void USetWalkSpeed::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			if (APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(Owner))
			{
				if (UCharacterMovementComponent* PlayerMovement = PlayerCharacter->GetCharacterMovement())
				{
					if (bIsFirst)
					{
						PlayerMovement->MaxWalkSpeed = 50.f;
					}
					else
					{
						PlayerMovement->MaxWalkSpeed = PlayerCharacter->GetBodyModeSpeed();
					}
				}
			}
		}
		
	}
}
