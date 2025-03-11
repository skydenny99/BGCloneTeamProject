// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Weapon/BaseFireGunAbility.h"

#include "Characters/PubgBaseCharacter.h"
#include "Components/BaseWeaponManageComponent.h"
#include "Components/ProjectilePoolComponent.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "HUD/HUDBase.h"
#include "HUD/PlayerHUD/PlayerHUD.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/BaseGun.h"
#include "Weapon/ProjectileBase.h"
#include "Weapon/WeaponBase.h"

UBaseFireGunAbility::UBaseFireGunAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UBaseFireGunAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                             const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		const AWeaponBase* Weapon = GetBaseCharacter()->GetEquipManageComponent()->GetCurrentWeapon();

		if (IsValid(Weapon) == false)
			Debug::PrintError("Weapon is invalid");
		if (Weapon->CanAttack() == false)
			Debug::PrintError("Weapon cannot fire");
		return IsValid(Weapon) && Weapon->CanAttack();
	}
	return false;
}

void UBaseFireGunAbility::PreActivate(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
	CurrentGun = Cast<ABaseGun>(GetBaseCharacter()->GetEquipManageComponent()->GetCurrentWeapon());
	// MOA 는 1inch per 100 yard이지만, 3cm per 100m로 근사
	MoaRadian = FMath::Atan(CurrentGun->GetGunSpec().Moa * 0.0003f);
	RandomStream = UBaseFunctionLibrary::SetSeedWithActivationKey(this);
}

void UBaseFireGunAbility::FireGun()
{
	// 서버
	if(HasAuthority(&CurrentActivationInfo))
	{
		CurrentGun->SetCurrentAmmo(CurrentGun->GetCurrentAmmo() - 1);
		//PlayWeaponMontage();
	}
	else
	{
		SpawnProjectileFunction();
		ApplyGunRecoil();
	}
	// 클라
	// if(GetBaseCharacter()->IsLocallyControlled())
	// {
	// 	if (APlayerController* PlayerController = Cast<APlayerController>(GetBaseCharacter()->GetController()))
	// 	{
	// 		if (AHUDBase* BaseHUD = Cast<AHUDBase>(PlayerController->GetHUD()))
	// 		{
	// 			BaseHUD->PlayerHUD->OnFireCurrentAmmo.Execute(GetCurrentGun()->GetCurrentMagBullet());
	// 			//BaseHUD->PlayerHUD->UpdateCurrentDecreaseAmmo(GetCurrentGun()->GetCurrentBullet());
	// 			Debug::Print(FString::Printf(TEXT("CurrentAmmo !!! %d"), GetCurrentGun()->GetCurrentMagBullet()));
	// 		}
	// 	}
	// }
}

void UBaseFireGunAbility::SpawnProjectileFunction()
{
	if (IsValid(CurrentGun) == false) return;
	const FVector SpawnLocation = (CurrentGun->GetWeaponSkeletalMeshComponent()->GetSocketLocation(FName("Gun_muzzleSocket")));
	const FVector AimLocation = UBaseFunctionLibrary::MakeCharacterAimVector(GetBaseCharacter());
	const FVector AimDirection = AimLocation - SpawnLocation;
	const FVector RandomAimDirection = RandomStream.VRandCone(AimDirection, MoaRadian);

	// 어빌리티 랜덤.함수로 수정
	const float OffsetDistance = 40.0f;

	const FRotator SpawnRotator = UKismetMathLibrary::FindLookAtRotation(SpawnLocation,SpawnLocation + RandomAimDirection);
	const FGunSpec& GunSpec = CurrentGun->GetGunSpec();
	AProjectileBase* SpawnProjectile = GetBaseCharacter()->GetProjectilePoolComponent()->GetProjectile();
	SpawnProjectile->SetActorLocationAndRotation(SpawnLocation + SpawnRotator.Vector() * OffsetDistance,SpawnRotator,false,nullptr,ETeleportType::None);
	SpawnProjectile->InitProjectile(GetBaseCharacter(),GunSpec);
	const FVector RandomTestDirection = RandomStream.VRandCone(FVector::UpVector, MoaRadian);
	// Debug::Print(FString::Printf(TEXT("Random Aim Direction, x: %f, y: %f, z: %f"), RandomTestDirection.X, RandomTestDirection.Y, RandomTestDirection.Z), FColor::Blue);
}


void UBaseFireGunAbility::ApplyGunRecoil()
{
	if (APubgBaseCharacter* BaseCharacter = GetBaseCharacter())
	{
		float VerticalRecoil = CurrentGun->GetGunSpec().VerticalRecoil;
		float HorizontalRecoil = CurrentGun->GetGunSpec().HorizontalRecoil;
		BaseCharacter->AddControllerPitchInput(FMath::RandRange(0.f, -VerticalRecoil));
		BaseCharacter->AddControllerYawInput(FMath::RandRange(-HorizontalRecoil, HorizontalRecoil));		
	}
	
}

void UBaseFireGunAbility::CheckFireMode(EFireMode& OutExec)
{
	if (IsValid(CurrentGun) == false) return;
	const FGameplayTag& CurrentFireMode = CurrentGun->GetCurrentFireMode();
	if (CurrentFireMode.MatchesTagExact(BaseGameplayTags::Weapon_Gun_FireMode_Single))
	{
		OutExec = EFireMode::Single;
	}
	else if (CurrentFireMode.MatchesTagExact(BaseGameplayTags::Weapon_Gun_FireMode_Burst))
	{
		OutExec = EFireMode::Burst;
	}
	else
	{
		OutExec = EFireMode::Auto;
	}
}

void UBaseFireGunAbility::CheckAmmo(bool& OutExec)
{
	OutExec = CurrentGun->CanAttack();
}

// void UBaseFireGunAbility::PlayWeaponMontage()
// {
// 	if (HasAuthority(&CurrentActivationInfo))
// 	{
// 		if (IsValid(CurrentGun))
// 		{
// 			CurrentGun->NetMulti_PlayFireMontage();
// 		}
// 	}
// }
