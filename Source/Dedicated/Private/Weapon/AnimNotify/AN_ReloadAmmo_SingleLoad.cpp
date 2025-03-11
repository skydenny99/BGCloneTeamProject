// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/AnimNotify/AN_ReloadAmmo_SingleLoad.h"

#include "Characters/PubgCharacter.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "Components/Renewal/InventoryManageComponent.h"
#include "Weapon/BaseGun.h"

void UAN_ReloadAmmo_SingleLoad::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       const FAnimNotifyEventReference& EventReference)
{
	if (APubgCharacter* PubgCharacter = MeshComp->GetOwner<APubgCharacter>())
	{
		if (PubgCharacter->IsLocallyControlled())
		{
			ABaseGun* BaseGun = Cast <ABaseGun>(PubgCharacter->GetEquipManageComponent()->GetCurrentWeapon());
			const int32 MaxBullets = BaseGun->GetMaxAmmo();
			const int32 CurrentBullets = BaseGun->GetCurrentAmmo();
			const int32 NeedBullets = MaxBullets - CurrentBullets;
			const uint8 BulletItemID = static_cast<uint8>(BaseGun->GetGunSpec().UsedBullet);
			const int32 CurrentAmount = PubgCharacter->GetInventoryComponent()->GetItemAmount(BulletItemID);
			if (NeedBullets > 0 && CurrentAmount > 0)
			{
				BaseGun->Server_AddAmmo(ReloadAmmoAmount);
				PubgCharacter->GetInventoryComponent()->Server_UseItem(BulletItemID, ReloadAmmoAmount);
				if (NeedBullets <= ReloadAmmoAmount)
				{
					if (MeshComp->GetAnimInstance())
					{
						MeshComp->GetAnimInstance()->Montage_JumpToSection(FName(ReloadEndSectionName));
					}
				}
			}
		}
	}
}
