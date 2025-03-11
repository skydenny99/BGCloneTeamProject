// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSet/PlayerAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseDebugHelper.h"
#include "BaseGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "Characters/PubgCharacter.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PubgPlayerController.h"
#include "GameFramework/PubgPlayerState.h"
#include "HUD/HUDBase.h"
#include "HUD/PlayerHUD/PlayerHUD.h"
#include "Weapon/WeaponBase.h"


UPlayerAttributeSet::UPlayerAttributeSet()
{
	InitCurrentHp(100);
	InitMaxHp(100);
	InitCurrentBoost(100);
	InitMaxBoost(100);
	InitDefence(0);
	InitHeadDefence(0);
	InitInventoryCapacity(70);
	InitMaxWalkSpeed(300);
}

void UPlayerAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	

	if (const FGameplayAbilityActorInfo* ActorInfo = GetActorInfo())
	{
		if (Attribute == GetMaxWalkSpeedAttribute())
		{
			if (UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(ActorInfo->MovementComponent))
			{
				CharacterMovement->MaxWalkSpeed = NewValue;
			}
		}
	}
}

void UPlayerAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Debug::Print(TEXT("Running on client"));
	//
	// //Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
	
	// else
	// {
	// 	Debug::Print(TEXT("PlayerController nullptr"));
	// }

	if (Data.EvaluatedData.Attribute == GetCurrentHpAttribute())
	{
		
		const float NewCurrentHp = FMath::Clamp(GetCurrentHp(), 0.0f, GetMaxHp());
		SetCurrentHp(NewCurrentHp);

	}

	if (Data.EvaluatedData.Attribute == GetCurrentBoostAttribute())
	{
		const float NewCurrentBoost = FMath::Clamp(GetCurrentBoost(), 0.0f, GetMaxBoost());
		SetCurrentBoost(NewCurrentBoost);
	}

	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute() && GetCurrentHp() > 0) // 아직 살아있으면 적용
	{
		float Damage = GetDamageTaken();
		SetDamageTaken(0.f);

		if (Damage > 0.f)
		{
			const float NewCurrentHp = GetCurrentHp() - Damage;
			SetCurrentHp(FMath::Clamp(NewCurrentHp, 0.0f, GetMaxHp()));
			Debug::Print("Take Damage", Damage);
		}

		if (GetCurrentHp() <= 0)
		{
			UAbilitySystemComponent* Source = Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();
			if (Source)
			{
				AActor* SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
				APubgPlayerController* SourceController = Cast<APubgPlayerController>(Source->AbilityActorInfo->PlayerController.Get());
				if (SourceController == nullptr && SourceActor != nullptr)
				{
					if (APawn* Pawn = Cast<APawn>(SourceActor))
					{
						SourceController = Pawn->GetController<APubgPlayerController>();
					}
				}

				// Use the controller to find the source pawn
				if (SourceController)
				{
					if (APubgPlayerState* PlayerState = SourceController->GetPlayerState<APubgPlayerState>())
					{
						if (APubgCharacter* TempPlayerCharacter = Cast<APubgCharacter>(SourceActor))
						{
							if (AWeaponBase* TempWeapon = TempPlayerCharacter->GetEquipManageComponent()->GetCurrentWeapon())
							{
								if (const FGunSpec* TempCurrentGun = UBaseFunctionLibrary::GetGunData(TempWeapon->GetItemID()))
								{
									PlayerState->SetPlayerGunID(TempCurrentGun->ItemID);
									Debug::Print(FString::Printf(TEXT("SourceActor Setup GunType Num : %s"),*UEnum::GetValueAsString(TempCurrentGun->GunType)));
								}
							}
						}
						PlayerState->SetEliminatedPlayerName(GetOwningActor()->GetName());
						PlayerState->SetSourcePlayerName(SourceActor->GetName());
						PlayerState->OnKill();
					}
				}
			
				if (APubgCharacter* Owner = Cast<APubgCharacter>(GetOwningActor()))
				{
					//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, BaseGameplayTags::GameplayEvent_Player_Dead, FGameplayEventData());
					Owner->PlayerDead();
				}
			}
		}
	}
}

void UPlayerAttributeSet::OnRep_CurrentHp(const FGameplayAttributeData& InCurrentHp)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, CurrentHp, InCurrentHp);
	if (APubgCharacter* Character = Cast<APubgCharacter>(GetOwningActor()))
	{
		if (APlayerController* PlayerController = Character->GetController<APlayerController>())
		{
			if (AHUDBase* BaseHUD = Cast<AHUDBase>(PlayerController->GetHUD()))
			{
				if (BaseHUD->PlayerHUD && IsValid(BaseHUD->PlayerHUD))
				{
					BaseHUD->PlayerHUD->UpdateHealth(GetCurrentHp(), GetMaxHp());
				}
			}
		}
				
	}
}

void UPlayerAttributeSet::OnRep_CurrentBoost(const FGameplayAttributeData& InCurrentBoost)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, CurrentBoost, InCurrentBoost);
}

void UPlayerAttributeSet::OnRep_Defence(const FGameplayAttributeData& InDefence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Defence, InDefence);
}

void UPlayerAttributeSet::OnRep_HeadDefence(const FGameplayAttributeData& InHeadDefence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, HeadDefence, InHeadDefence);
}

void UPlayerAttributeSet::OnRep_InventoryCapacity(const FGameplayAttributeData& InInventoryCapacity)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, InventoryCapacity, InInventoryCapacity);
}

void UPlayerAttributeSet::OnRep_MaxWalkSpeed(const FGameplayAttributeData& InMaxWalkSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MaxWalkSpeed, InMaxWalkSpeed);
}
