// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PubgPlayerController.h"

#include "BaseDebugHelper.h"
#include "AbilitySystem/PubgGameplayEffectContext.h"
#include "Characters/PubgBaseCharacter.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "GameFramework/PubgGameState.h"
#include "HUD/HUDBase.h"
#include "HUD/PlayerHUD/PlayerHUD.h"
#include "HUD/PlayerHUD_Child/CrosshairWidget.h"
#include "HUD/PlayerHUD_Child/PlayerStateSlot/BreathSlotWidget.h"

class UPlayerHUD;

void APubgPlayerController::BeginPlay()
{
	
}

APubgPlayerController::APubgPlayerController()
{

}

void APubgPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
	if (IsLocalController())
	{
		if (APubgBaseCharacter* BaseCharacter = Cast<APubgBaseCharacter>(P))
		{
			BaseCharacter->GetBaseAbilitySystemComponent()->InitAbilityActorInfo(BaseCharacter, BaseCharacter);
			Debug::Print("Client Init avatar info");
		}
	}
	
}

void APubgPlayerController::RemoveGameplayTagToControlledPawn_Implementation(FGameplayTag Tag)
{
	if (GetPawn())
	{
		UBaseFunctionLibrary::RemovePlayGameTagFromActor(GetPawn(), Tag);
	}
}

void APubgPlayerController::AddGameplayTagToControlledPawn_Implementation(FGameplayTag Tag)
{
	if (GetPawn())
	{
		UBaseFunctionLibrary::AddPlaygameTagToActor(GetPawn(), Tag);
	}
}

void APubgPlayerController::ApplyDamageToTarget_Implementation(TSubclassOf<UGameplayEffect> DamageEffectClass, const FHitResult& Hit, const FGunDamageSpecInfo& SpecInfo,  APubgBaseCharacter* Target)
{
	APubgBaseCharacter* MyPawn = Cast<APubgBaseCharacter>(GetPawn());
	if (MyPawn == nullptr) return;
	FGameplayEffectContextHandle ContextHandle;
	ContextHandle.AddSourceObject(MyPawn);
	ContextHandle.AddInstigator(MyPawn, MyPawn);
			
			
	UBaseAbilitySystemComponent* ASC = MyPawn->GetBaseAbilitySystemComponent();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass, 1.f, ContextHandle);
	if (FPubgGameplayEffectContext* EffectContext = static_cast<FPubgGameplayEffectContext*>(SpecHandle.Data->GetContext().Get()))
	{
		Debug::Print("Base damage in server", SpecInfo.BaseDamage);
		EffectContext->AddHitResult(Hit, true);
		// FGunDamageSpecInfo DamageSpecInfo;
		// DamageSpecInfo.BaseDamage = 50.f;
		// DamageSpecInfo.GunType = EGunType::AR;
		EffectContext->SetGunDamageSpecInfo(SpecInfo);
	}
	ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, Target->GetBaseAbilitySystemComponent());
}


void APubgPlayerController::OnPlayerDead_Implementation()
{
	if (APubgGameState* State = Cast<APubgGameState>(GetWorld()->GetGameState()))
	{
		State->OnPlayerDead();
	}
}
