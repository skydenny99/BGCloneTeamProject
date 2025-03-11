// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ProjectilePoolComponent.h"

#include "BaseDebugHelper.h"
#include "Characters/PubgCharacter.h"
#include "Weapon/ProjectileBase.h"


// Sets default values for this component's properties
UProjectilePoolComponent::UProjectilePoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

AProjectileBase* UProjectilePoolComponent::GetProjectile()
{
	if (Projectiles.IsEmpty()) ExpandPool();
	return Projectiles.Pop();
}

void UProjectilePoolComponent::OnDespawnProjectile(AProjectileBase* Projectile)
{
	Projectile->ActivateProjectile(false);
	Projectiles.Push(Projectile);
}

void UProjectilePoolComponent::BeginPlay()
{
	Super::BeginPlay();
	
	APubgCharacter* PlayerCharacter = Cast<APubgCharacter>(GetOwner());
	if (PlayerCharacter->IsLocallyControlled())
	{
		ExpandPool();
	}
}

void UProjectilePoolComponent::ExpandPool()
{
	if (ProjectileClass == nullptr) return;
	for (int32 i = 0; i < 100; i++)
	{
		AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass);
		Projectile->ActivateProjectile(false);
		Projectiles.Push(Projectile);
	}
}

void UProjectilePoolComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	for (auto& Projectile : Projectiles)
	{
		Projectile->Destroy();
	}
}

