// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PubgBaseCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "BaseGamePlayTags.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PlayerAttributeSet.h"
#include "DataAssets/Input/DataAsset_PubgInputConfig.h"
#include "Components/BaseInputComponent.h"
#include "Characters/PubgCharacter.h"
#include "BaseDebugHelper.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "AnimInstances/PubgAnimInstance.h"
#include "Components/BaseGearManageComponent.h"
#include "Components/BasePickUpComponent.h"
#include "Components/BaseTimelineComponent.h"
#include "Components/BaseWeaponManageComponent.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/ProjectilePoolComponent.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "Components/Renewal/InventoryManageComponent.h"
#include "Components/Renewal/PickUpManageComponent.h"
#include "DataAssets/StartupData/DataAsset_StartupBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PubgPlayerController.h"
#include "HUD/HUDBase.h"
#include "HUD/PlayerHUD/PlayerHUD.h"
#include "Net/UnrealNetwork.h"
#include "Spawn/BasePlane.h"


// Sets default values
APubgBaseCharacter::APubgBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->bReceivesDecals = false;

	BaseAbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>("BaseAbilitySystemComponent");
	BaseAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>("UPlayerAttributeSet");
	EquipManageComponent = CreateDefaultSubobject<UEquipManageComponent>("EquipManageComponent");
	EquipManageComponent->SetIsReplicated(true);
	
	PickUpManageComponent = CreateDefaultSubobject<UPickUpManageComponent>("PickUpManageComponent");
	InventoryComponent = CreateDefaultSubobject<UInventoryManageComponent>("InventoryComponent");
	InventoryComponent->SetIsReplicated(true);
	
	ProjectilePoolComponent = CreateDefaultSubobject<UProjectilePoolComponent>("ProjectilePoolComponent");
	BaseTimelineComponent = CreateDefaultSubobject<UBaseTimelineComponent>("BaseTimelineComponent");
	
	OverrideInputComponentClass = UBaseInputComponent::StaticClass();

	bReplicates = true;
	GetCharacterMovement()->SetIsReplicated(true);
	BaseAbilitySystemComponent->SetIsReplicated(true);
	SetReplicatingMovement(true);
	GetCharacterMovement()->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void APubgBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APubgBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	checkf(InputConfigDataAsset, TEXT("Forgot to Input Config First File"));

	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
	
	UBaseInputComponent* BaseInputComponent = CastChecked<UBaseInputComponent>(PlayerInputComponent); 
	
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &APubgCharacter::Input_Move);
	
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_Move, ETriggerEvent::Completed, this, &APubgCharacter::Input_ReleaseMove);
	BaseInputComponent->BindAbilityAction(InputConfigDataAsset, this, &APubgBaseCharacter::OnTriggerAbilityAction);
	BaseInputComponent->BindAttackAction(InputConfigDataAsset, this);
}
void APubgBaseCharacter::Input_Move(const FInputActionValue& InputValue)
{
	const FVector2D MovementVector = InputValue.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);	
	}
}

TArray<FGameplayAbilitySpecHandle> APubgBaseCharacter::GrantAbilities(const TArray<TSubclassOf<UBaseGameplayAbility>>& Abilities)
{
	TArray<FGameplayAbilitySpecHandle> Specs;
	if (HasAuthority() == false || IsValid(BaseAbilitySystemComponent) == false)
	{
		return Specs;
	}

	for (const TSubclassOf<UBaseGameplayAbility>& Ability : Abilities)
	{
		if (IsValid(Ability) == false) continue;
		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = this;
		AbilitySpec.Level = 1;
		BaseAbilitySystemComponent->GiveAbility(AbilitySpec);
		Specs.Add(AbilitySpec.Handle);
	}
	return Specs;
}

void APubgBaseCharacter::ClearAbilities(const TArray<FGameplayAbilitySpecHandle>& AbilitySpecHandles)
{
	if (HasAuthority() == false || IsValid(BaseAbilitySystemComponent) == false)
	{
		return;
	}
	for (auto& Handle : AbilitySpecHandles)
	{
		BaseAbilitySystemComponent->ClearAbility(Handle);
	}
}

UAbilitySystemComponent* APubgBaseCharacter::GetAbilitySystemComponent() const
{
	return GetBaseAbilitySystemComponent();
}

void APubgBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	ensureMsgf(!StartupData.IsNull(), TEXT("Forgot to Assigned startup data to %s"), *GetName());

	if (GetNetMode() == NM_DedicatedServer)
	{
		if (BaseAbilitySystemComponent)
		{
			BaseAbilitySystemComponent->InitAbilityActorInfo(this,this);
			Debug::Print("Server Init avatar info");
			UDataAsset_StartupBase* StartUp = StartupData.LoadSynchronous();
			if (StartUp)
			{
				GrantAbilities(StartUp->ActivateOnGivenAbilities);
				GrantAbilities(StartUp->ReactiveAbilities);
				for (auto& Effect : StartUp->StartupEffects)
				{
					if (Effect == nullptr) continue;
					BaseAbilitySystemComponent->ApplyGameplayEffectToSelf(Effect.GetDefaultObject(), 1.f, BaseAbilitySystemComponent->MakeEffectContext());
				}
			}
		}
		SetOwner(NewController);
	}
}

void APubgBaseCharacter::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);
	if (HasAuthority())
	{
		const FRotator& ContRot = GetControlRotation();
		BaseAimRotation = FVector_NetQuantize(ContRot.Pitch, ContRot.Yaw, ContRot.Roll);
	}
}

void APubgBaseCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(APubgBaseCharacter, BaseAimRotation, COND_SimulatedOnly);
}

void APubgBaseCharacter::RegisterOnAttributeChanged_OneParam(const FGameplayAttribute& CurrentData, FOnAttributeChangedOneParam Delegate)
{
	BaseAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CurrentData).AddLambda([Delegate](const FOnAttributeChangeData& ChangeData)
	{
		Delegate.ExecuteIfBound(ChangeData.NewValue);
	});
}

void APubgBaseCharacter::RegisterOnAttributeChanged_TwoParams(const FGameplayAttribute& CurrentData, const FGameplayAttribute& MaxData, FOnAttributeChangedTwoParams Delegate)
{
	BaseAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CurrentData).AddLambda([this, MaxData, Delegate](const FOnAttributeChangeData& ChangeData)
	{
		bool IsFound = false;
		float Value = UAbilitySystemBlueprintLibrary::GetFloatAttribute(this, MaxData, IsFound);
		if (IsFound)
		{
			Delegate.ExecuteIfBound(ChangeData.NewValue, Value);
		}
	});
}

void APubgBaseCharacter::PlayerDead_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SetLifeSpan(5.f);
	if (HasAuthority())
	{
		PickUpManageComponent->DiscardAllItems();
	}
	
	if (IsLocallyControlled())
	{
		if (APubgPlayerController* PubgController = GetController<APubgPlayerController>())
		{
			if (ULocalPlayer* LocalPlayer = PubgController->GetLocalPlayer())
			{
			
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					Subsystem->RemoveMappingContext(InputConfigDataAsset->DefaultMappingContext);
				}
			}

			Debug::Print("Player Dead");
			PubgController->OnPlayerDead();
		}
	}
	
}

void APubgBaseCharacter::Input_ReleaseMove()
{
}

void APubgBaseCharacter::OnTriggerAbilityAction(FGameplayTag InputTag)
{
	if (BaseAbilitySystemComponent)
	{
		BaseAbilitySystemComponent->TryActivateAbilityByTag(InputTag);
	}
}

bool APubgBaseCharacter::IsMontagePlaying()
{
	return false; 
}

void APubgBaseCharacter::SkipCurrentMontage()
{
}


FRotator APubgBaseCharacter::GetBaseAimRotation() const
{
	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		return FRotator(BaseAimRotation.X, BaseAimRotation.Y, BaseAimRotation.Z);
	}
	return Super::GetBaseAimRotation();
}
