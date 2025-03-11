// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Weapon/BaseFireShotgunAbility.h"

#include "BaseDebugHelper.h"
#include "BaseType/BaseStructType.h"
#include "Characters/PubgBaseCharacter.h"
#include "Components/ProjectilePoolComponent.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/ProjectileBase.h"


void UBaseFireShotgunAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	PelletSpreadRadian = FMath::DegreesToRadians(PelletSpreadDegree);
}

void UBaseFireShotgunAbility::SpawnProjectileFunction()
{
	if (IsValid(CurrentGun) == false) return;
	const FVector SpawnLocation = (CurrentGun->GetWeaponSkeletalMeshComponent()->GetSocketLocation(FName("Gun_muzzleSocket")));
	const FVector AimLocation = UBaseFunctionLibrary::MakeCharacterAimVector(GetBaseCharacter());
	const FVector AimDirection = AimLocation - SpawnLocation;
	const float OffsetDistance = 40.0f;
	const FGunSpec& GunSpec = CurrentGun->GetGunSpec();
	for (int i = 0; i < PelletCount; i++)
	{
		const FVector RandomAimDirection = RandomStream.VRandCone(AimDirection, PelletSpreadRadian);
		const FRotator SpawnRotator = UKismetMathLibrary::FindLookAtRotation(SpawnLocation,SpawnLocation + RandomAimDirection);
		AProjectileBase* SpawnProjectile = GetBaseCharacter()->GetProjectilePoolComponent()->GetProjectile();
		SpawnProjectile->SetActorLocationAndRotation(SpawnLocation + SpawnRotator.Vector() * OffsetDistance,SpawnRotator,false,nullptr,ETeleportType::None);
		SpawnProjectile->InitProjectile(GetBaseCharacter(),GunSpec);
		Debug::Print(FString::Printf(TEXT("Random Aim Direction, x: %f, y: %f, z: %f"), RandomAimDirection.X, RandomAimDirection.Y, RandomAimDirection.Z), FColor::Blue);
	}
}
