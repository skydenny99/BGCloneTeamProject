// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/AnimNotify/ANS_AttachMagazine.h"

#include "Characters/PubgBaseCharacter.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "Weapon/BaseGun.h"
#include "Weapon/Parts/WeaponPartsBase.h"


void UANS_AttachMagazine::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (APubgBaseCharacter* OwningCharacter = Cast<APubgBaseCharacter>(MeshComp->GetOwner()))
	{
		if (ABaseGun* CurrentWeapon = Cast<ABaseGun>(OwningCharacter->GetEquipManageComponent()->GetCurrentWeapon()))
		{
			if (AWeaponPartsBase* Mag = CurrentWeapon->GetPartsInSlot(BaseGameplayTags::Item_Parts_Magazine, true))
			{
				const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
				Mag->AttachToComponent(MeshComp, AttachmentRules, FName("item_l"));
			}
		}
	}
}

void UANS_AttachMagazine::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (APubgBaseCharacter* OwningCharacter = Cast<APubgBaseCharacter>(MeshComp->GetOwner()))
	{
		if (ABaseGun* CurrentWeapon = Cast<ABaseGun>(OwningCharacter->GetEquipManageComponent()->GetCurrentWeapon()))
		{
			if (AWeaponPartsBase* Mag = CurrentWeapon->GetPartsInSlot(BaseGameplayTags::Item_Parts_Magazine, true))
			{
				const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
				Mag->AttachToComponent(CurrentWeapon->GetWeaponSkeletalMeshComponent(), AttachmentRules, FName("Gun_magazinesocket"));
			}
		}
	}
}
