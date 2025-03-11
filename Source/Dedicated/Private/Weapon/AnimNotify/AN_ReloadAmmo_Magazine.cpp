// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/AnimNotify/AN_ReloadAmmo_Magazine.h"

#include "Characters/PubgCharacter.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "Components/Renewal/InventoryManageComponent.h"
#include "Weapon/BaseGun.h"


void UAN_ReloadAmmo_Magazine::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
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
				const int32 ReloadAmount = FMath::Min(CurrentAmount, NeedBullets);
				BaseGun->Server_AddAmmo(ReloadAmount);
				PubgCharacter->GetInventoryComponent()->Server_UseItem(BulletItemID, ReloadAmount);
			}
		}
	}
}
