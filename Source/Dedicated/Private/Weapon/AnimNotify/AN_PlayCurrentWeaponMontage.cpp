// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/AnimNotify/AN_PlayCurrentWeaponMontage.h"

#include "BaseDebugHelper.h"
#include "Characters/PubgBaseCharacter.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "Weapon/WeaponBase.h"


void UAN_PlayCurrentWeaponMontage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
	if (IsValid(WeaponMontage) == false) return;
	if (APubgBaseCharacter* OwningCharacter = MeshComp->GetOwner<APubgBaseCharacter>(); IsValid(OwningCharacter))
	{
		if (OwningCharacter->IsLocallyControlled() == false) return;
		Debug::Print(FString::Printf(TEXT("Play Weapon Montage in Character : %s"), *OwningCharacter->GetActorNameOrLabel()));
		if (AWeaponBase* CurrentWeapon = OwningCharacter->GetEquipManageComponent()->GetCurrentWeapon(); IsValid(CurrentWeapon))
		{
			Debug::Print(FString::Printf(TEXT("Play Weapon Montage in Weapon : %s"), *CurrentWeapon->GetActorNameOrLabel()));
			Debug::Print(FString::Printf(TEXT("Play Weapon Montage in Animation : %s"), *Animation->GetFullName()));
			USkeletalMeshComponent* SkeletalMeshComp = CurrentWeapon->GetWeaponSkeletalMeshComponent();
			if (IsValid(SkeletalMeshComp))
			{
				SkeletalMeshComp->GetAnimInstance()->Montage_Play(WeaponMontage);
			}
		}
	}
}
