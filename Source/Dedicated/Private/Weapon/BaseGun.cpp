// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseGun.h"

#include "BaseDebugHelper.h"
#include "Characters/PubgBaseCharacter.h"
#include "Components/BaseWeaponManageComponent.h"
#include "Components/Renewal/InventoryManageComponent.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Subsystem/PubgDataTableManageSubsystem.h"
#include "Weapon/WeaponPartsComponent.h"
#include "Weapon/Parts/WeaponPartsBase.h"


class AHUDBase;

ABaseGun::ABaseGun()
{
	
	// MagMesh = CreateDefaultSubobject<UStaticMeshComponent>("MagMesh");
	// MagMesh->SetupAttachment(WeaponMeshComp, FName("Gun_magazinesocket"));
	//
	// SightMesh = CreateDefaultSubobject<UStaticMeshComponent>("SightMesh");
	// SightMesh->SetupAttachment(WeaponMeshComp, FName("Gun_sightSocket"));
	// SightMesh->SetIsReplicated(true);
	//
	// MuzzleMesh = CreateDefaultSubobject<UStaticMeshComponent>("MuzzleMesh");
	// MuzzleMesh->SetupAttachment(WeaponMeshComp, FName("Gun_muzzleSocket"));
	//
	// GripMesh  = CreateDefaultSubobject<UStaticMeshComponent>("GripMesh");
	// GripMesh->SetupAttachment(WeaponMeshComp, FName("gun_gripSocket"));
	//
	// StockMesh = CreateDefaultSubobject<UStaticMeshComponent>("StockMesh");
	// StockMesh->SetupAttachment(WeaponMeshComp, FName("Gun_stockSocket"));
	//
	// DefaultSightMesh = CreateDefaultSubobject<UStaticMeshComponent>("DefaultSightMesh");
	// DefaultSightMesh->SetVisibility(false);
	// DefaultSightMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FireModeIndex = 0;
	CurrentAmmo = 30;
	// SetCurrentMagBullet(GunSpec.MaxBulletCount);
	
	
}

void ABaseGun::BeginPlay()
{
	Super::BeginPlay();
	
	// Init Gun Spec
	if (const FGunSpec* FoundGunSpec = UBaseFunctionLibrary::GetGunData(FCString::Atoi(*GunName)))
	{
		GunSpec = *FoundGunSpec;
		MaxAmmo = GunSpec.MaxBulletCount;
	}
	else
	{
		Debug::PrintError(FString::Printf(TEXT("Can't find Gun spec name: %s"), *GunName));
	}
}


void ABaseGun::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ABaseGun, MaxAmmo, COND_InitialOrOwner);
	DOREPLIFETIME_CONDITION(ABaseGun, CurrentAmmo, COND_InitialOrOwner);
	DOREPLIFETIME_CONDITION(ABaseGun, FireModeIndex, COND_InitialOrOwner);
	DOREPLIFETIME_CONDITION(ABaseGun, ReloadSpeedRate, COND_InitialOrOwner);
}


 // ability에서 처리하므로 주석
#pragma region Fire Logic
// void ABaseGun::SetFireModeStrategy(const FGameplayTag& FireModeTag)
// {
// 	if (GunSpec.FireModes.HasTagExact(FireModeTag))
// 	{
// 		if (FireModeStrategies.Contains(FireModeTag) == false)
// 		{
// 			if (FireModeTag.MatchesTagExact(BaseGameplayTags::Weapon_Gun_FireMode_Single))
// 			{
// 				FireModeStrategies.Add(FireModeTag, NewObject<UPubgGunFire_Single>());
// 			}
// 			if (FireModeTag.MatchesTagExact(BaseGameplayTags::Weapon_Gun_FireMode_Burst))
// 			{
// 				FireModeStrategies.Add(FireModeTag, NewObject<UPubgGunFire_Burst>());
// 			}
// 			if (FireModeTag.MatchesTagExact(BaseGameplayTags::Weapon_Gun_FireMode_Auto))
// 			{
// 				FireModeStrategies.Add(FireModeTag, NewObject<UPubgGunFire_Auto>());
// 			}
// 		}
// 		CurrentFireModeStrategy = FireModeStrategies[FireModeTag];
// 		//CurrentFireModeStrategy->InitGun(this);
// 	}
// }
//
// void ABaseGun::BeginFire()
// {
// 	if (IsValid(WeaponOwner) && WeaponOwner->IsLocallyControlled())
// 	{
// 		if (IsValid(CurrentFireModeStrategy))
// 		{
// 			CurrentFireModeStrategy->BeginFire();
// 		}
// 	}
// }
//
// void ABaseGun::StopFire()
// {
// 	if (IsValid(WeaponOwner) && WeaponOwner->IsLocallyControlled())
// 	{
// 		if (IsValid(CurrentFireModeStrategy))
// 		{
// 			CurrentFireModeStrategy->EndFire();
// 		}
// 	}
// }
#pragma endregion



bool ABaseGun::CanAttack() const
{
	return CurrentAmmo > 0;
}

bool ABaseGun::CanReload() const
{
	// return Super::CanReload();
	return CurrentAmmo < MaxAmmo;
	
}

void ABaseGun::OnPutAway()
{
	Super::OnPutAway();
	OnUpdateCurrentAmmoDelegate.Clear();
}


FGameplayTag ABaseGun::GetCurrentFireMode() const
{
	check(GunSpec.FireModes.IsEmpty() == false);
	if (GunSpec.FireModes.IsValidIndex(FireModeIndex) == false) return GunSpec.FireModes.First();
	return GunSpec.FireModes.GetByIndex(FireModeIndex);
}

void ABaseGun::SwitchFireMode()
{
	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
		check(GunSpec.FireModes.IsEmpty() == false);
		FireModeIndex = (FireModeIndex + 1) % GunSpec.FireModes.Num();
		//SetFireModeStrategy(GetCurrentFireMode());
	}
}

void ABaseGun::Server_AddAmmo_Implementation(uint8 InAmount)
{
	const uint8 Sum = CurrentAmmo + InAmount;
	CurrentAmmo = FMath::Min(Sum, MaxAmmo);
}

FName ABaseGun::GetGunSocketZoomInfo(float& OutVal)
{
	// if (SightMesh->GetStaticMesh())
	// {
	// 	FString SightName =	GetSightMesh()->GetStaticMesh()->GetName();
	// 	Debug::Print(SightName);
	//
	// 	if (SightName == "EOTech")
	// 	{
	// 		OutVal = 60;
	// 		return FName("Gun_SightCameraSocket");
	// 	}
	// 	else if (SightName == "CQBSS")
	// 	{
	// 		Debug::Print(FString::Printf(TEXT("GunSocketZoomInfo -- CQBSS")));
	// 		OutVal = 13;
	// 		if (UMaterialInstance* MI_Zoom = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(),
	// 			nullptr, TEXT("/Game/Weapon/Accessories/CQBSS_/Mesh/CQBSS_Zoom_Inst.CQBSS_Zoom_Inst"))))
	// 		{
	// 			SightMesh->SetMaterial(0, MI_Zoom);
	// 		}
	// 		return FName("Gun_ScopeCameraSocket");
	// 	}
	// 	else
	// 	{
	// 		OutVal = 60;
	// 		return FName("Gun_IronSightCameraSocket");
	// 	}
	// }
	// else
	// {
	// 	OutVal = 60;
	// 	Debug::Print(TEXT("No Static Mesh Scope !!!!!!!"));
	// 	return FName("Gun_IronSightCameraSocket");
	// 	
	// 	
	// }
	const FString& SocketName = WeaponPartsComponent->GetSightInfo(OutVal);
	WeaponPartsComponent->OnFppZoomIn();
	return FName(SocketName);
}

void ABaseGun::GunZoomOutReset()
{
	WeaponPartsComponent->OnFppZoomOut();
	// if (SightMesh->GetStaticMesh())
	// {
	// 	FString SightName =	GetSightMesh()->GetStaticMesh()->GetName();
	// 	Debug::Print(SightName);
	// 	if (SightName == "CQBSS")
	// 	{
	// 		if (UMaterialInstance* MI_Reset = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(),
	// 			nullptr, TEXT("/Game/Weapon/Accessories/CQBSS_/Mesh/CQBSSBase_Inst.CQBSSBase_Inst"))))
	// 		{
	// 			SightMesh->SetMaterial(0, MI_Reset);
	// 		}
	// 	}
	// }
}


void ABaseGun::NetMulti_PlayFireMontage_Implementation()
{
	// Debug::Print("Play Montage");
	GetWeaponAnimInstance()->Montage_Play(FireMontage);
}

void ABaseGun::NetMulti_PlayReloadMontage_Implementation()
{
	GetWeaponAnimInstance()->Montage_Play(ReloadMontage);
}

void ABaseGun::OnRep_MaxAmmo()
{
	OnUpdateWeaponInfoDelegate.Broadcast(this);
}


void ABaseGun::OnRep_CurrentAmmo()
{
	OnUpdateCurrentAmmoDelegate.Broadcast(CurrentAmmo);
}

void ABaseGun::OnRep_FireMode()
{
	OnUpdateWeaponInfoDelegate.Broadcast(this);
}

float ABaseGun::GetFireRate() const
{
	const FGameplayTag& CurrentFireMode = GetCurrentFireMode();
	if (GunSpec.FireRate.Contains(CurrentFireMode))
	{
		return GunSpec.FireRate[CurrentFireMode];
	}
	return 0.1f; // default
}

void ABaseGun::IncreaseMaxAmmo(uint8 InAmount)
{
	if (HasAuthority())
	{
		MaxAmmo = GunSpec.MaxBulletCount + InAmount;
		if (CurrentAmmo > MaxAmmo)
		{
			if (APubgBaseCharacter* OwningCharacter = GetOwner<APubgBaseCharacter>())
			{
				int32 _;
				OwningCharacter->GetInventoryComponent()->AcquireItem(static_cast<uint8>(GunSpec.UsedBullet), CurrentAmmo - MaxAmmo, _);
			}
		}
		CurrentAmmo = FMath::Min(CurrentAmmo, MaxAmmo);
		
	}
}
