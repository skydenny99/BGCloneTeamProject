// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileBase.h"

#include "BaseDebugHelper.h"
#include "AbilitySystem/PubgGameplayEffectContext.h"
#include "Characters/PubgBaseCharacter.h"
#include "Components/ProjectilePoolComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/PubgPlayerController.h"


// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// 충돌 처리 설정
	// RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	//CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComp->SetCollisionProfileName("Projectile_Bullet");
	RootComponent = CollisionComp;
	
	// Projectile Movement Component 추가
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 0.0f; // 초기 속도
	ProjectileMovement->MaxSpeed = 0.0f;     // 최대 속도
	ProjectileMovement->bRotationFollowsVelocity = true; // 발사체 방향을 속도에 맞춤
	ProjectileMovement->bShouldBounce = false;          // 튀는 효과 끔
	ProjectileMovement->ProjectileGravityScale = 0.0f;  // 중력 설정
	
	ProjectileMovement->bAutoActivate = false;



	// 충돌 이벤트 바인딩
	OnActorHit.AddDynamic(this, &AProjectileBase::OnHit);

	

}


void AProjectileBase::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Shooter.IsValid() && Shooter->IsLocallyControlled() == false) return;
	if (OtherActor == Shooter)	{ return;}
	
	if (APubgBaseCharacter* TargetCharacter = Cast<APubgBaseCharacter>(OtherActor))
	{
		if (Shooter.IsValid() && Shooter == TargetCharacter)
		{
			return;
		}
		
		// 킬로그 관련 정보 추가 필요 - 일단 instigator로 처리
		if (Shooter.IsValid() && Shooter != TargetCharacter) // 쏜 사람이 valid한 경우만 처리 (만약 러브샷을 허용해야 할 경우, context에 킬러 정보를 담는 방식을 고려)
		{
			APubgPlayerController* PC = Shooter->GetController<APubgPlayerController>();
			if (PC)
			{
				PC->ApplyDamageToTarget(DamageEffectClass, Hit, DamageSpecInfo, TargetCharacter);
			}
			//ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetCharacter->GetBaseAbilitySystemComponent());
		}
		
	}
}

void AProjectileBase::ActivateProjectile(bool InActive)
{
	SetActorHiddenInGame(InActive == false);
	if (InActive)
	{
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ProjectileMovement->SetUpdatedComponent(RootComponent);
	}
	else
	{
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ProjectileMovement->SetUpdatedComponent(nullptr);
	}
}

void AProjectileBase::InitProjectile(APubgBaseCharacter* InShooter, const FGunSpec& Spec)
{
	Shooter = InShooter;
	DamageSpecInfo.BaseDamage = Spec.Damage;
	DamageSpecInfo.GunType = Spec.GunType;
	ProjectileMovement->InitialSpeed = Spec.BulletSpeed * 100.f;
	ProjectileMovement->MaxSpeed = Spec.BulletSpeed * 100.f;
	ProjectileMovement->ProjectileGravityScale = 1.0f; 
	ActivateProjectile(true);
	ProjectileMovement->Activate();
	ProjectileMovement->Velocity = GetActorForwardVector() * ProjectileMovement->MaxSpeed;
}

