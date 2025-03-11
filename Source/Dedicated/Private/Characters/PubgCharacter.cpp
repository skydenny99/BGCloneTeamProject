// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PubgCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BaseInputComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "BaseDebugHelper.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AttributeSet/PlayerAttributeSet.h"
#include "AnimInstances/PubgAnimInstance.h"
#include "BaseType/BaseEnumType.h"
#include "Weapon/BaseGun.h"
#include "Characters/PubgInventoryCharacter.h"
#include "Components/BaseInteractComponent.h"
#include "Components/BasePickUpComponent.h"
#include "Components/BaseTimelineComponent.h"
#include "Components/BaseWeaponManageComponent.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "Components/Renewal/PickUpManageComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PubgPlayerController.h"
#include "HUD/HUDBase.h"
#include "HUD/PlayerHUD/PlayerHUD.h"
#include "HUD/PlayerHUD_Child/GearWidget.h"
#include "HUD/PlayerHUD_Child/Interact.h"
#include "HUD/UI/InventoryMainWidget.h"
#include "Item/BaseFieldItem.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Spawn/BasePlane.h"
#include "Curves/CurveFloat.h"
#include "HUD/PlayerHUD_Child/SummaryWidget.h"
#include "MapInteraction/BaseDoor.h"


APubgCharacter::APubgCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Setup CapsuleComponent 
	GetCapsuleComponent()->InitCapsuleSize(35.f, 90.f);


	// Setup ControllerRotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	bUpdateControllerRotationYaw = false;

	// Setup CameraSpringArm
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(GetRootComponent());
	CameraSpringArm->TargetArmLength = 200.0f;
	CameraSpringArm->SetRelativeLocation(Init_Location_CameraSpringArm);
	CameraSpringArm->bUsePawnControlRotation = true;

	// Setup CameraComponent.
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	FPP_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPP_Camera"));
	FPP_Camera->SetupAttachment(GetMesh(), (FName("camera_fpp")));
	FPP_Camera->SetRelativeRotation(FRotator(90,-90,0));
	// FPP_Camera->SetOrthoNearClipPlane()

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	BodyMesh -> SetupAttachment(GetMesh());
	BodyMesh->SetLeaderPoseComponent(GetMesh());

	LegMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegMesh"));
	LegMesh-> SetupAttachment(GetMesh());
	LegMesh->SetLeaderPoseComponent(GetMesh());

	FeetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FeetMesh"));
	FeetMesh-> SetupAttachment(GetMesh());
	FeetMesh->SetLeaderPoseComponent(GetMesh());

	
	 
	
	// Setup CharacterMovement
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = StandSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 100.0f;

	// Setup Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Asset/Character/Female/Mesh/Female.Female"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -100.0f), FRotator(0, -90.0f, 0));
	
	
	static ConstructorHelpers::FClassFinder<APubgInventoryCharacter> BP_InventoryCharacter(TEXT("/Game/_BP/Character/Inventory/BP_PubgInventoryCharacter"));
	if (BP_InventoryCharacter.Succeeded())
	{
		InventoryCharacterClass = BP_InventoryCharacter.Class;
	}

	// 1인칭 줌 관련 변수 초기화
	bIsButtonPressed = false;
	ShortPressThreshold = 0.3f;
}


void APubgCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Debug::Print(FString::Printf(TEXT("APubgCharacter -- BeginPlay()")));

	

	
	if (!PubgAnimInstance)
		PubgAnimInstance = Cast<UPubgAnimInstance>(GetMesh()->GetAnimInstance());

	// HUD 보다 캐릭터가 먼저 생성될 때 문제발생 가능
	if (!PlayerHUDBase)
	{
		if (APubgPlayerController* PlayerController = Cast<APubgPlayerController>(GetController()))
		{
			if (AHUDBase* HUDBase = Cast<AHUDBase>(PlayerController->GetHUD()))
			{
				PlayerHUDBase = HUDBase;
				PlayerHUDBase->InitDeligate();
				PlayerHUDBase = HUDBase;
			}
		}
	}
	
	// 초반 카메라 앵글 제한
	LimitCameraAngle(PlayerBodyMode);

	//인벤용 캐릭터 스폰
	if (APubgInventoryCharacter* _InventoryCharacter = GetWorld()->SpawnActor<APubgInventoryCharacter>(InventoryCharacterClass,FVector(2890,60930,7600),FRotator(0.f,30.f,-40.f)))
	{
		InventoryCharacter = _InventoryCharacter;
		InventoryCharacter->SetOwningPawn(this);
	}
	else
	{
		Debug::Print("Failed to SpawnInventoryCharacter!",FColor::Red);
	}
	
}

void APubgCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsFreeFalling && bHasPlaneExit && !bIsFreeFallingLock) // 자유 낙하시. 일정 거리 도달했는데도 낙하산을 안폈을 때 강제 펴기.
	{
		float TempDistanceToGround = GetDistanceToGround();

		if (TempDistanceToGround <= 2000.f)
		{
			bIsFreeFallingLock = true;
			Server_FreeFalling();
			Debug::Print("Tick :: 2000.f ", TempDistanceToGround);
		}
	}
	else if (PubgAnimInstance && bHasPlaneExit) // 낙하산 상태로 일정거리 도달했는데도. 낙하산 안접으면 강제 접기.
	{
		if (!PubgAnimInstance->bParachuteExit)
		{
			float TempDistanceToGround = GetDistanceToGround();
			if (TempDistanceToGround <= 300.f)
			{
				Server_FreeFalling();
				Debug::Print("Tick :: 300.f ", TempDistanceToGround);
			}
		}
	}
}

void APubgCharacter::Input_Look_Custom(const FInputActionValue& InputValue)
{
	FVector2D LookAxisVector = InputValue.Get<FVector2D>();

	// 감도 보정값 적용
	// SensitivityX = 0.5f;  // 좌우 감도
	// SensitivityY = 0.5f; // 상하 감도

	LookAxisVector.X *= MouseSensitivityX;
	LookAxisVector.Y *= MouseSensitivityY;

	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}
	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}

		
}

void APubgCharacter::SetMouseSensitivity(float NewSensitivityX, float NewSensitivityY)
{
	MouseSensitivityX = NewSensitivityX;
	MouseSensitivityY = NewSensitivityY;
}



void APubgCharacter::OnRep_IsLeaning()
{
	if (UPubgAnimInstance* AnimInstance = Cast<UPubgAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->mLeaning = mIsLeaning;
	}

	if (PubgAnimInstance)
		PubgAnimInstance->mLeaning = mIsLeaning;
}

void APubgCharacter::OnRep_FreeFalling()
{
	if (PubgAnimInstance)
	{
		PubgAnimInstance->ActiveParachuteExit();
		Debug::Print("APubgCharacter::OnRep_FreeFalling");
	}
}

void APubgCharacter::Multicast_SetLeaning_Implementation(const float _NewValue)
{
	if (PubgAnimInstance)
	{
		if (PubgAnimInstance->mLeaning != _NewValue)
		{
			PubgAnimInstance->mLeaning = _NewValue;
			mIsLeaning = _NewValue;
			OnChangeCameraLocation(GetPoseCameraLocation(), GetTargetArmLength());
			if (mIsLeaning != 0.f)
			{
				CurrentMaxWalkSpeed = GetBodyModeSpeed() / 2.f;
			}
			else
			{
				CurrentMaxWalkSpeed = GetBodyModeSpeed();
			}
		}
	}
}


void APubgCharacter::Server_SetLeaning_Implementation(const float _NewValue)
{
	Multicast_SetLeaning(_NewValue);
}

void APubgCharacter::Client_PlaneInputContext_Implementation(const bool bIsPlane)
{
	if (IsLocallyControlled())
	{
		if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					if (bIsPlane)
					{
						// 비행기 컨텍스트를 높은 우선순위로 추가
						Subsystem->AddMappingContext(InputConfigDataAsset->PlaneMappingContext, 1);
						
						// 기존 플레이어 컨텍스트의 우선순위를 낮춤 (사실상 비활성화)
						Subsystem->RemoveMappingContext(InputConfigDataAsset->DefaultMappingContext);
					}
					else
					{
						// 플레이어 컨텍스트를 다시 높은 우선순위로 설정
						Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
						
						// 기존 비행기 컨텍스트의 우선순위를 낮춤 (사실상 비활성화)
						Subsystem->RemoveMappingContext(InputConfigDataAsset->PlaneMappingContext);
					}
				}
				else
				{
					Debug::Print("LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() Failed");
				}
			}
			else
			{
				Debug::Print("PlayerController->GetLocalPlayer() Failed");
			}
		}
		else
		{
			Debug::Print("Cast<APlayerController>(GetController()) Failed");
		}
	}
	else
	{
		Debug::Print("Is Not LocallyPlayerController");
	}
	
}

FVector APubgCharacter::GetPoseCameraLocation()
{
	FVector NewLocation = FVector::Zero();

	// 각 포즈별, 줌했을 경우와 안했을 경우의 값을 Return
	switch (PlayerBodyMode)
	{
	case EPlayerActionMode::Crouch:
		if (bIsZoomIn)
		{
			if (mIsLeaning == 0.f)
			{
				NewLocation = Init_Location_ZoomCamera;
				NewLocation.Z -= 48.f;
			}
			else if (mIsLeaning == -1.f)
			{
				NewLocation = Init_Location_ZoomCamera;
				NewLocation.Z -= 48.f;
				NewLocation.Y -= 100.f;
			}
			else if (mIsLeaning == 1.f)
			{
				NewLocation = Init_Location_ZoomCamera;
				NewLocation.Z -= 48.f;
				NewLocation.Y += 20.f;
			}
		}
		else
		{
			if (mIsLeaning == 0.f)
			{
				NewLocation = Init_Location_CameraSpringArm;
				NewLocation.Z -= 30.f;
			}
			else if (mIsLeaning == -1.f)
			{
				NewLocation = Init_Location_CameraSpringArm;
				NewLocation.Z -= 30.f;
				NewLocation.Y -= 50.f;
			}
			else if (mIsLeaning == 1.f)
			{
				NewLocation = Init_Location_CameraSpringArm;
				NewLocation.Z -= 30.f;
				NewLocation.Y += 50.f;
			}
		}
		break;
	case EPlayerActionMode::Prone:
		if (bIsZoomIn)
		{
			NewLocation = Init_Location_ZoomCamera;
			NewLocation.Z -= 100.f;
		}
		else
		{
			NewLocation = Init_Location_CameraSpringArm;
			NewLocation.Z -= 100.f;
		}
		break;
	case EPlayerActionMode::Stand:
		if (bIsZoomIn)
		{
			if (mIsLeaning == 0.f)
				NewLocation = Init_Location_ZoomCamera;
			else if (mIsLeaning == -1.f)
			{
				NewLocation = Init_Location_ZoomCamera;
				NewLocation.Y -= 100.f;
			}
			else if (mIsLeaning == 1.f)
			{
				NewLocation = Init_Location_ZoomCamera;
				NewLocation.Y += 20.f;
			}
		}
		else
		{
			if (mIsLeaning == 0.f)
				NewLocation = Init_Location_CameraSpringArm;
			else if (mIsLeaning == -1.f)
			{
				NewLocation = Init_Location_CameraSpringArm;
				NewLocation.Y -= 50.f;
			}
			else if (mIsLeaning == 1.f)
			{
				NewLocation = Init_Location_CameraSpringArm;
				NewLocation.Y += 50.f;
			}
		}
		break;
	default:
		break;
	}

	return NewLocation;
}


void APubgCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UBaseInputComponent* BaseInputComponent = CastChecked<UBaseInputComponent>(PlayerInputComponent);

	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_Move,
												ETriggerEvent::Triggered, this, &APubgCharacter::Input_MoveCheck);

	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_Look,
												ETriggerEvent::Triggered, this, &APubgCharacter::Input_Look_Custom);
	
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_Look,
												ETriggerEvent::Triggered, this, &APubgCharacter::Input_LookCheck);
	
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_Jump,
	                                          ETriggerEvent::Started, this, &APubgCharacter::PubgJump);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_Jump,
	                                          ETriggerEvent::Completed, this, &APubgCharacter::PubgStopJump);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_ActionMode_Crouch,
	                                          ETriggerEvent::Started, this, &APubgCharacter::Input_Crouch);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_ActionMode_Rifle,
	                                          ETriggerEvent::Started, this, &APubgCharacter::Input_Rifle);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_ActionMode_Prone,
	                                          ETriggerEvent::Started, this, &APubgCharacter::Input_Prone);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_Sprint,
	                                          ETriggerEvent::Triggered, this, &APubgCharacter::Input_LShift);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_Sprint,
	                                          ETriggerEvent::Completed, this, &APubgCharacter::Input_LShift_Release);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_Walk,
	                                          ETriggerEvent::Triggered, this, &APubgCharacter::Input_LCtrl);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_Walk,
	                                          ETriggerEvent::Completed, this, &APubgCharacter::Input_LCtrl_Release);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_Look,
	                                          ETriggerEvent::Triggered, this, &APubgCharacter::CheckCharacterAngle);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_Look,
	                                          ETriggerEvent::Triggered, this, &APubgCharacter::CheckCloseActor);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_Move,
	                                          ETriggerEvent::Started, this, &APubgCharacter::CheckNotTurn);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_Move,
	                                          ETriggerEvent::Triggered, this, &APubgCharacter::GetPlayerMoveDirection);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_ZoomIn,
	                                          ETriggerEvent::Triggered, this, &APubgCharacter::FlipZoomIn);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_ZoomIn,
	                                          ETriggerEvent::Completed, this, &APubgCharacter::FlipZoomIn_Release);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_ZoomIn,
	 										  ETriggerEvent::Triggered, this, &APubgCharacter::OnZoomButtonPressed);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_ZoomIn,
											  ETriggerEvent::Completed, this, &APubgCharacter::OnZoomButtonReleased);
	
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_Leaning,
	                                          ETriggerEvent::Triggered, this, &APubgCharacter::Input_Leaning);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_Interact,
											  ETriggerEvent::Completed, this, &APubgCharacter::Input_Interacting);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_Inventory,
											  ETriggerEvent::Completed, this, &APubgCharacter::ToggleInventory);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTags::InputTag_ExitPlane,
											  ETriggerEvent::Completed, this, &APubgCharacter::Input_ExitPlane);


	
}

void APubgCharacter::Server_SetZoom_Implementation(bool InZoomIn)
{
	bIsZoomIn = InZoomIn;
}

void APubgCharacter::Server_ActiveControllerYaw_Implementation()
{
	bUpdateControllerRotationYaw = true;
}


void APubgCharacter::CheckNotTurn_Implementation()
{
	if (bHasTurn && !bIsLanding)
	{
		PubgNotTurn();
	}
}

void APubgCharacter::Input_Crouch_Implementation()
{
	switch (PlayerBodyMode)
	{
	case EPlayerActionMode::Crouch:
		ChangePlayerBodyMode(EPlayerActionMode::Stand);
		break;
	default:
		ChangePlayerBodyMode(EPlayerActionMode::Crouch);
		break;
	}
}

void APubgCharacter::Input_Rifle_Implementation()
{
	switch (PlayerEquipMode)
	{
	case EPlayerActionMode::Rifle:
		ChangePlayerEquipMode(EPlayerActionMode::Unarmed);
		break;
	default:
		ChangePlayerEquipMode(EPlayerActionMode::Rifle);
		break;
	}
}

void APubgCharacter::Input_LCtrl_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = GetBodyModeSpeed() / 2;
	
	CurrentMaxWalkSpeed = GetBodyModeSpeed() / 2;
}

void APubgCharacter::Input_LCtrl_Release_Implementation()
{
	if (HasAuthority())
	{
		GetCharacterMovement()->MaxWalkSpeed = GetBodyModeSpeed();
		CurrentMaxWalkSpeed = GetBodyModeSpeed();
	}
}

void APubgCharacter::Input_LShift_Release_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = GetBodyModeSpeed();
	CurrentMaxWalkSpeed = GetBodyModeSpeed();

	if (PubgAnimInstance)
		PubgAnimInstance->bAming = true;
}

void APubgCharacter::Input_Prone_Implementation()
{
	if (PlayerBodyMode != EPlayerActionMode::Prone)
	{
		// When over 80% of Maximum Speed, Run to Prone Animation
		if (GetVelocity().Size2D() >= (GetBodyModeSpeed() * 2) * 0.8)
		{
			if (PubgAnimInstance)
				PubgAnimInstance->bSprintProne = true;
		}
		ChangePlayerBodyMode(EPlayerActionMode::Prone);
		return;
	}

	if (PlayerBodyMode == EPlayerActionMode::Prone)
	{
		ChangePlayerBodyMode(EPlayerActionMode::Stand);
	}
}

void APubgCharacter::PubgJump_Implementation()
{
	if (PlayerBodyMode == EPlayerActionMode::Stand)
	{
		bHasJump = true;
		
		if (UKismetMathLibrary::VSizeXY(GetVelocity()) != 0.f)
		{
			if (PubgAnimInstance)
			{
				PubgAnimInstance->bMoveJump = true;
				FString Text = FString::Printf(TEXT("AnimInstance bMoveJump : %hhd"), PubgAnimInstance->bMoveJump);
			}
		}
		ACharacter::Jump();
	}
	else
	{
		if (PlayerBodyMode == EPlayerActionMode::Crouch)
			Input_Crouch();
		else if (PlayerBodyMode == EPlayerActionMode::Prone)
			Input_Prone();
	}
}

void APubgCharacter::PubgStopJump_Implementation()
{
	if (bHasJump)
	{
		bHasJump = false;
		ACharacter::StopJumping();
	}
}

void APubgCharacter::CheckCharacterAngle_Implementation()
{
	FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetBaseAimRotation(), GetActorRotation());
	
	if (!bHasTurn)
		bHasTurn = true;
	
	if ((DeltaRotation.Yaw > 75.f || DeltaRotation.Yaw < -75.f) && !GetCharacterMovement()->IsFalling() && !bIsLanding
		&& GetVelocity().Size2D() == 0.f)
	{
		if (HasAuthority())
		{
			Server_OnPubgTurn();
		}
		else
		{
			PubgTurn();
		}
	
		bHasTurn = false;
		if (PubgAnimInstance)
		{
			PubgAnimInstance->bHasTurn = true;
		}
	}
}

void APubgCharacter::Input_LShift_Implementation()
{
	GetPlayerMoveDirection();
	if (PlayerMoveDirection >= 0.7f && GetPlayerBodyMode() != EPlayerActionMode::Prone && !bIsZoomIn && !FPP_MODE)
	{
		GetCharacterMovement()->MaxWalkSpeed = GetBodyModeSpeed() * 2;
		if (!bUseControllerRotationYaw)
			bUseControllerRotationYaw = true;
		
		CurrentMaxWalkSpeed = GetBodyModeSpeed() * 2;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = GetBodyModeSpeed();
		CurrentMaxWalkSpeed = GetBodyModeSpeed();
	}
	if (PubgAnimInstance)
		PubgAnimInstance->bAming = false;
}


void APubgCharacter::PlayPickUpMontage_Implementation(EPickMode PickMode)
{
	if (PubgAnimInstance && !bIsPickUp)
	{
		PickupMontage = PickupMontageMap[PlayerEquipMode].MontageMap[PlayerBodyMode].Montages[NearActor.PickMode].Montages[0] ?PickupMontageMap[PlayerEquipMode].MontageMap[PlayerBodyMode].Montages[NearActor.PickMode].Montages[0] : nullptr;
		
		if (PickupMontage)
		{
			bIsPickUp = true;
			PubgAnimInstance->Montage_Play(PickupMontage);
		}
		else
		{
			Debug::Print("mMontage is nullptr");
		}
		
		if (GetWorld()->GetTimerManager().IsTimerActive(PickUpTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(PickUpTimerHandle);
			GetWorld()->GetTimerManager().SetTimer(PickUpTimerHandle, this,&APubgCharacter::PickupCompleted,2.f, false);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(PickUpTimerHandle, this,&APubgCharacter::PickupCompleted,2.f, false);
		}
					
	}
}

void APubgCharacter::SetFPP_MODE_Implementation(bool bVal)
{
	if (HasAuthority())
	{
		// 서버의 FPP_MODE 값을 변경
		FPP_MODE = bVal; 

		// 클라이언트들에게 변경사항을 동기화
		Multicast_SetFPP_MODE(bVal);
	}
}

void APubgCharacter::Multicast_SetFPP_MODE_Implementation(bool bVal)
{
	// 모든 클라이언트에서 FPP_MODE 값을 변경
	FPP_MODE = bVal; 
}

void APubgCharacter::ChangePlayerBodyMode_Implementation(const EPlayerActionMode NewPlayerBodyMode)
{
	if (PlayerBodyMode == NewPlayerBodyMode) return;
	if (GetCharacterMovement()->IsFalling()) return;
	if (NewPlayerBodyMode == EPlayerActionMode::Unarmed || NewPlayerBodyMode == EPlayerActionMode::Rifle ||
		NewPlayerBodyMode == EPlayerActionMode::Melee || NewPlayerBodyMode == EPlayerActionMode::Grenade)
		return;

	PlayerBodyMode = NewPlayerBodyMode;

	switch (NewPlayerBodyMode)
	{
	case EPlayerActionMode::Prone:
		GetCharacterMovement()->MaxWalkSpeed = GetBodyModeSpeed();
		OnChangeCameraLocation(GetPoseCameraLocation(), GetTargetArmLength());
		LimitCameraAngle(NewPlayerBodyMode);
		break;
	case EPlayerActionMode::Stand:
		GetCharacterMovement()->MaxWalkSpeed = GetBodyModeSpeed();
		OnChangeCameraLocation(GetPoseCameraLocation(), GetTargetArmLength());
		LimitCameraAngle(NewPlayerBodyMode);
		break;
	case EPlayerActionMode::Crouch:
		GetCharacterMovement()->MaxWalkSpeed = GetBodyModeSpeed();
		OnChangeCameraLocation(GetPoseCameraLocation(), GetTargetArmLength());
		LimitCameraAngle(NewPlayerBodyMode);
		break;
	default:
		break;
	}
	
	CurrentMaxWalkSpeed = GetBodyModeSpeed();
	
	if (!PubgAnimInstance)
		PubgAnimInstance = Cast<UPubgAnimInstance>(GetMesh()->GetAnimInstance());
	if (PubgAnimInstance)
	{
		PubgAnimInstance->SetOwningSpeed(GetBodyModeSpeed());
		PubgAnimInstance->PlayerBodyMode = PlayerBodyMode;
	}
}

void APubgCharacter::ChangePlayerEquipMode_Implementation(const EPlayerActionMode NewPlayerEquipMode)
{
	// NewPlayerMode is Equal to Current Value?
	if (PlayerEquipMode == NewPlayerEquipMode) return;

	// NewPlayerMode is Equip Mode?
	if (NewPlayerEquipMode == EPlayerActionMode::Crouch || NewPlayerEquipMode == EPlayerActionMode::Prone ||
		NewPlayerEquipMode == EPlayerActionMode::Stand)
		checkf(true, TEXT("Change Equip Mode is Not Valid"));


	PlayerEquipMode = NewPlayerEquipMode;
	FlipZoomIn_Release();

	if (PubgAnimInstance)
		PubgAnimInstance->PlayerEquipMode = NewPlayerEquipMode;
}


AHUDBase* APubgCharacter::GetHUDBase()
{
	if (PlayerHUDBase == nullptr)
	{
		if (APubgPlayerController* PlayerController = Cast<APubgPlayerController>(GetController()))
		{
			if (AHUDBase* HUDBase = Cast<AHUDBase>(PlayerController->GetHUD()))
			{
				PlayerHUDBase = HUDBase;
				PlayerHUDBase->InitDeligate();
			}
		}
		else
		{
			Debug::Print("APubgCharacter::GetHUDBase :: GetController is Failed");
		}
	}
	return PlayerHUDBase;
}

void APubgCharacter::Server_FreeFalling_Implementation()
{
	Multicast_FreeFalling();
}

void APubgCharacter::Multicast_FreeFalling_Implementation()
{
	// 맨처음 자유낙하 상태일 때. F키를 누르면 낙하산 호출
	if (bIsFreeFalling)
	{
		if (PubgAnimInstance)
		{
			bIsFreeFalling = false;
			PubgAnimInstance->bFreeFalling = false;
		}
	
		// 낙하산 스폰
		if (ParachuteClass)
		{
			if (Parachute = GetWorld()->SpawnActor<AActor>(ParachuteClass, FVector::Zero(), FRotator::ZeroRotator))
			{
				if (GetMesh()->DoesSocketExist(FName("backpack")))
				{
					Parachute->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("backpack"));
					Parachute->SetActorRotation(FRotator(GetActorRotation().Pitch-135.f, GetActorRotation().Yaw, 0.f));
					if (BaseTimelineComponent)
					{
						BaseTimelineComponent->StartCameraLocation(GetPoseCameraLocation(), 1000.f, CameraSpringArm);
					}
					// TimeLine 함수
					if (BaseTimelineComponent)
						BaseTimelineComponent->StartParachuteScale(Parachute);
					
					GetCharacterMovement()->GravityScale = 0.05f/2;
					GetCharacterMovement()->Velocity.Z *= 0.05f/2;

					//F Interact Widget 비활성화 = [낙하산 펴기]
					Client_ToggleInteractWidget(false);
				}
			}
		}
	}
	else
	{
		if (!bIsFreeFalling && !PubgAnimInstance->bParachuteExit)
		{
			if (IsValid(Parachute))
			{
				Parachute->Destroy();
			}
			PubgAnimInstance->bParachuteExit = true;
			if (BaseTimelineComponent)
			{
				BaseTimelineComponent->StartCameraLocation(GetPoseCameraLocation(),GetTargetArmLength(), CameraSpringArm);
			}
			GetCharacterMovement()->GravityScale = 1.75f;
			GetCharacterMovement()->Velocity.Z = 0.f;
			Client_DeActiveInteractWidget();
		}
			
	}
}

void APubgCharacter::StartFreeFalling()
{
	if (HasAuthority())
		Multicast_StartFreeFalling();
	else
		Server_StartFreeFalling();
}


void APubgCharacter::Server_StartFreeFalling_Implementation()
{
	Multicast_StartFreeFalling();
}

void APubgCharacter::Multicast_StartFreeFalling_Implementation()
{
	if (PubgAnimInstance)
	{
		PubgAnimInstance->ActiveParachuteExit();
		Client_ActiveFreeFallingUI();
	}
	else
	{
		Debug::Print("StartFreeFalling :: GetController is Failed",FColor::Red);
	}
}

void APubgCharacter::Client_ActiveFreeFallingUI_Implementation()
{
	if (GetHUDBase())
	{
		GetHUDBase()->GetPlayerHUD()->TriggerFreeFallingWidget(true);
	}
	else
	{
		Debug::Print("Client_ActiveFreeFallingUI_Implementation :: GetHUDBase is Failed",FColor::Red);

	}
	// 비행기에서 떨어졌는지?
	bHasPlaneExit = true;
}

void APubgCharacter::Client_DeActiveInteractWidget_Implementation()
{
	if (GetHUDBase())
	{
		GetHUDBase()->GetPlayerHUD()->ToggleInteractWidget(false);
		GetHUDBase()->GetPlayerHUD()->TriggerFreeFallingWidget(false);
	}
	else
	{
		Debug::Print("Client_DeActiveInteractWidget :: Failed",FColor::Red);
	}
}

void APubgCharacter::Client_ToggleInteractWidget_Implementation(const bool _Active)
{
	if (IsLocallyControlled())
	{
		if (_Active)
		{
			GetHUDBase()->GetPlayerHUD()->ToggleInteractWidget(true);
			return;
		}
		else
		{
			GetHUDBase()->GetPlayerHUD()->ToggleInteractWidget(false);
			return;
		}
	}
	
}

void APubgCharacter::Client_CrosshairTrigger_Implementation(const bool _Active)
{
	if (_Active)
	{
		GetHUDBase()->ToogleCrosshairWidget(true);
	}
	else
	{
		GetHUDBase()->ToogleCrosshairWidget(false);
	}
}

float APubgCharacter::GetDistanceToGround()
{
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FVector End = Start + (FVector::DownVector*11000.f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHitGround = GetWorld()->LineTraceSingleByChannel(Hit,Start,End,ECC_Visibility,Params);

	if (bHitGround)
	{
		return (Start - Hit.ImpactPoint).Size();
	}

	return 11000.f;
}

float APubgCharacter::GetBodyModeSpeed() const
{
	switch (PlayerBodyMode)
	{
	case EPlayerActionMode::Prone:
		return ProneSpeed;
	case EPlayerActionMode::Stand:
		return StandSpeed;
	case EPlayerActionMode::Crouch:
		return CrouchSpeed;
	default:
		return 0;
	}
}

void APubgCharacter::UpdateInventory()
{
	if (GetHUDBase())
	{
		GetHUDBase()->GetInventoryMainWidget()->UpdateAroundItem();
		GetHUDBase()->GetInventoryMainWidget()->UpdateInventory();
		GetHUDBase()->GetInventoryMainWidget()->UpdateWeapon();
	}
	else
	{
		Debug::Print(FString::Printf(TEXT("GetHUDBase is nullptr from %s"), *this->GetName()));
	}
}

void APubgCharacter::Multicast_OnPubgTurn_Implementation()
{
	PubgTurn();
}

bool APubgCharacter::Server_OnPubgTurn_Validate()
{
	return true;
}

void APubgCharacter::Server_OnPubgTurn_Implementation()
{
	Multicast_OnPubgTurn();
}

void APubgCharacter::OnChangeCameraLocation(const FVector& _Camera_Location, const float _TargetArmLenth)
{
	if (BaseTimelineComponent)
		BaseTimelineComponent->StartCameraLocation(_Camera_Location, _TargetArmLenth, CameraSpringArm);
}


void APubgCharacter::CheckCloseActor()
{
	GetCloseActorToScreenCenter();
	
	if (NearActor.NearActor)
	{
		if (NearActor.BeforeActor && NearActor.BeforeActor != NearActor.NearActor)
		{
			if (UBaseInteractComponent* BeforeBaseInteract = NearActor.BeforeActor->FindComponentByClass<UBaseInteractComponent>())
			{
				BeforeBaseInteract->ToggleOverlayMaterial(false);
				
				// UI
				if (GetHUDBase())
					PlayerHUDBase->GetPlayerHUD()->ToggleInteractWidget(false);
			}
			NearActor.BeforeActor = nullptr;
		}
		
		if (UBaseInteractComponent* BaseInteract = NearActor.NearActor->FindComponentByClass<UBaseInteractComponent>())
		{
			if (!BaseInteract->GetOnMaterial())
			{
				BaseInteract->ToggleOverlayMaterial(true);
				NearActor.BeforeActor = NearActor.NearActor;

				// UI
				if (GetHUDBase())
				{
					// Name을 전달해줘야 하기 때문에. 여기에 3개의 함수를 사용하도록 설계
					PlayerHUDBase->GetPlayerHUD()->ToggleInteractWidget(true);
					PlayerHUDBase->GetPlayerHUD()->GetInteractWidget()->SetDestText(BaseInteract->GetItemName());
					PlayerHUDBase->GetPlayerHUD()->SetInteractWidgetSize();
				}
			}
			
		}
	}
	else if (NearActor.BeforeActor) // CActor가 Null인 이유가 오버렙 중이지만 먼곳을 바라보고 있을 경우
	{
		if (UBaseInteractComponent* BeforeBaseInteract = NearActor.BeforeActor->FindComponentByClass<UBaseInteractComponent>())
		{
			BeforeBaseInteract->ToggleOverlayMaterial(false);
			// UI
			if (GetHUDBase())
				PlayerHUDBase->GetPlayerHUD()->ToggleInteractWidget(false);
		}
		NearActor.BeforeActor = nullptr;
	}
}

void APubgCharacter::InitCloseActor_Implementation()
{
	if (GetHUDBase())
	{
		PlayerHUDBase->GetPlayerHUD()->ToggleInteractWidget(false);
		NearActor.NearActor = nullptr;
		NearActor.BeforeActor = nullptr;
	}
}

bool APubgCharacter::CheckItemActorOnOverlapActors()
{
	TArray<AActor*> OverlapActors;
	GetOverlappingActors(OverlapActors, AActor::StaticClass());

	if (OverlapActors.Num() > 0)
	{
		for (auto& OverlapActor : OverlapActors)
		{
			if (UBaseInteractComponent* BaseInteract = OverlapActor->FindComponentByClass<UBaseInteractComponent>())
			{
				return true;
			}
		}
		return false;
	}

	return false;
}

float APubgCharacter::GetTargetArmLength()
{
	if (bIsZoomIn)
		return ZoomTargetArmLength;
	else
		return mTargetArmLength;
}

bool APubgCharacter::IsMontagePlaying()
{
	if (PubgAnimInstance)
	{
		return PubgAnimInstance->GetSlotMontageGlobalWeight(FName("DefaultGroup")) > 0;
	}
	return false;
}

void APubgCharacter::SkipCurrentMontage()
{
	if (PubgAnimInstance)
	{
		PubgAnimInstance->Montage_StopGroupByName(0.f, FName("DefaultGroup"));
	}
}

void APubgCharacter::Input_Leaning(const FInputActionValue& InputValue)
{
	const float InputActionValue = InputValue.Get<float>();

	if (HasAuthority())
	{
		Multicast_SetLeaning(InputActionValue);
	}
	else
	{
		Server_SetLeaning(InputActionValue);
	}
}

void APubgCharacter::Input_Interacting()
{
	if (IsLocallyControlled())
	{
		ABaseFieldItem* FieldItem = GetNearestFieldItem();
		if (FieldItem)
		{
			PickUpManageComponent->PickUp(FieldItem);
		}
	}

	if (!PubgAnimInstance->bParachuteExit)
	{
		if (GetDistanceToGround() <= 10000.f)
		{
			if (HasAuthority())
				Multicast_FreeFalling();
			else
				Server_FreeFalling();

		}
	}
	
	// 문 상호 작용 
	if (HasAuthority())
	{
		NetMulticast_FindDoor();
	}
	else
	{
		Server_DoorInput_Interacting();
	}
}

void APubgCharacter::Server_DoorInput_Interacting_Implementation()
{
	NetMulticast_FindDoor();
}
void APubgCharacter::NetMulticast_FindDoor_Implementation()
{
	FHitResult HitResult;
	const FVector Start = FollowCamera->GetComponentLocation();
	const FVector End = Start + FollowCamera->GetForwardVector() * InteractLineTraceLength;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);
	if (ABaseDoor* Door = Cast<ABaseDoor> (HitResult.GetActor()))
	{
		Door->Character= this;
		Door->OnInteract();
		Debug::Print(FString::Printf(TEXT("Sever PlayerActor Name , %p"), this),FColor::Orange);
	}
	
}
void APubgCharacter::Input_ExitPlane()
{
	PlaneExitPlayer();
	GetHUDBase()->ToogleCrosshairWidget(true);
}

void APubgCharacter::AttachPlane_Implementation()
{
	AActor* Plane = UGameplayStatics::GetActorOfClass(GetWorld(),ABasePlane::StaticClass());
	if (const ABasePlane* BasePlane = Cast<ABasePlane>(Plane))
	{
		FName SocketName = FName("RidingSocket");
		
		this->SetActorEnableCollision(false);
		this->SetActorHiddenInGame(true);
		
		if (USkeletalMeshComponent* SkeletalMesh = BasePlane->FindComponentByClass<USkeletalMeshComponent>())
		{
			if (SkeletalMesh->DoesSocketExist(SocketName))
			{
				// 1. 소켓의 월드 좌표 가져오기
				const FTransform SocketTransform = SkeletalMesh->GetSocketTransform(SocketName, ERelativeTransformSpace::RTS_World);
				// 2. 먼저 소켓의 위치로 캐릭터 이동
				this->SetActorTransform(SocketTransform);
				this->SetActorRotation(SocketTransform.GetRotation());
				// 3. 소켓에 어태치
				this->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
				// 4. 어태치 상태 동기화
				bIsAttachedToPlane = true;
				AttachedSocketTransform = SocketTransform;
			}
			else
			{
				Debug::PrintError("SkeletalMesh->DoesSocketExist(SocketName) FALL");
			}
		}
		else
		{
			Debug::PrintError("BasePlane->FindComponentByClass<USkeletalMeshComponent>() FALL");
		}
	}
	else
	{
		Debug::PrintError("Plane FALL");
	}
}


ABaseFieldItem* APubgCharacter::GetNearestFieldItem()
{
	if (IsValid(PickUpManageComponent) == false) return nullptr;
	TArray<ABaseFieldItem*> OverlapItems = PickUpManageComponent->GetPickUpCandidates();
	if (OverlapItems.IsEmpty()) return nullptr;
	OverlapItems.Sort([this](const ABaseFieldItem& a, const ABaseFieldItem& b)
	{
		return FVector::DistSquaredXY(a.GetActorLocation(), GetActorLocation())
		< FVector::DistSquaredXY(b.GetActorLocation() ,GetActorLocation());
	});

	return OverlapItems[0];
}

void APubgCharacter::OnRep_PlayerBodyMode()
{
	ChangePlayerBodyMode(PlayerBodyMode);
}

void APubgCharacter::GetCloseActorToScreenCenter()
{
	// 오버랩 된 Actor 가져오기
	TArray<AActor*> OverlapActors;

	// AActor 클래스만 필터링 추후 Item에 대한 클래스 생성되면 해당 클래스로 필터링 변경
	GetOverlappingActors(OverlapActors, AActor::StaticClass());

	// 화면 가로 세로 변수 선언
	int32 ViewportWidth, ViewportHeight;

	// 화면 가로 새로 값 할당
	Cast<APubgPlayerController>(GetController())->GetViewportSize(ViewportWidth, ViewportHeight);

	// 값을 FVector로 전환
	FVector2D ScreenSize(ViewportWidth, ViewportHeight);

	// 화면 정중앙 좌표
	FVector2D ScreenCenter = ScreenSize * 0.5f; 
	
	AActor* ClosestActor = nullptr;

	// 가장 작은 거리 저장
	float MinDistance = FLT_MAX;
	for (AActor* OverlappedActor : OverlapActors)
	{
		if (!OverlappedActor) continue;

		// 액터의 월드 위치 가져오기
		FVector WorldLocation = OverlappedActor->GetActorLocation();
		FVector2D ScreenPosition;

		// 월드 좌표 -> 스크린 좌표 변환
		bool bProjected = Cast<APubgPlayerController>(GetController())->ProjectWorldLocationToScreen(WorldLocation, ScreenPosition);
        
		if (bProjected)
		{
			
			// Screen상에서 정중앙과, 액터의 거리 계산
			float Distance = FVector2D::Distance(ScreenPosition, ScreenCenter);
			//Debug::Print("Distance: %f", Distance);
			
			// 최소 거리 갱신
			if (Distance < 100.f && Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestActor = OverlappedActor;
			}
			else if (Distance >= 100.f && Distance < MinDistance) // 바라보고 있지 않을경우 해제 
			{
				ClosestActor = nullptr;
			}
		}
	}
	
	NearActor.NearActor =  ClosestActor ? ClosestActor : nullptr;
}

void APubgCharacter::Multicast_SetPickMode_Implementation(const float LocationZ)
{
	if (UKismetMathLibrary::Abs( GetActorLocation().Z - LocationZ) < 20.f)
		NearActor.PickMode = EPickMode::Middle;
	else if (GetActorLocation().Z > LocationZ)
		NearActor.PickMode = EPickMode::Low;
	else
		NearActor.PickMode = EPickMode::High;
}

void APubgCharacter::Server_SetPickMode_Implementation(const float LocationZ)
{
	Multicast_SetPickMode(LocationZ);
}

bool APubgCharacter::PlaneExitPlayer_Validate()
{
	return true;
}

void APubgCharacter::PlaneExitPlayer_Implementation()
{
	AActor* Plane = UGameplayStatics::GetActorOfClass(GetWorld(),ABasePlane::StaticClass());
	if (ABasePlane* BasePlane = Cast<ABasePlane>(Plane))
	{
		if (CanExit)
		{
			FVector NewLocation = BasePlane->GetActorLocation();
			NewLocation.Z -= 340.f;
			// 플레이어의 위치를 비행기 아래 100 로 변경
			if (BasePlane)
			{
				UE_LOG(LogTemp, Error, TEXT("CameraTargetChanged"));

				// 2 플레이어의 위치를 비행기 아래로 설정
				this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				SetActorLocation(NewLocation);
				//this->CameraSpringArm->TargetArmLength = 200.0f;
				this->SetActorHiddenInGame(false);
				this->SetActorEnableCollision(true);
				if (UCharacterMovementComponent* MovementComp = this->GetCharacterMovement())
				{
					MovementComp->SetMovementMode(MOVE_Walking);
				}

				// 자유낙하 시작
				StartFreeFalling();
			}
			CanExit = false;
			Client_PlaneInputContext(false);
		}
		else
		{
			Debug::Print("Can,t Exit",FColor::Yellow);
		}
	}
	else
	{
		Debug::Print("Plane Null",FColor::Red);
	}
	
}


void APubgCharacter::Client_InventoryChangeEquip_Implementation()
{
	if (InventoryCharacter)
	{
		if (AWeaponBase* CurrentWeapon = GetEquipManageComponent()->GetCurrentWeapon())
		{
			Debug::Print("APubgCharacter::Client_InventoryChangeEquip Cast WeaponBase");
			if (ABaseGun* CurrentGunBase = Cast<ABaseGun>(CurrentWeapon))
			{
				Debug::Print("APubgCharacter::Client_InventoryChangeEquip Cast CurrentGunBase");
				EGunName Gun = CurrentGunBase->GetGunSpec().GunName;
				Debug::Print(FString::Printf(TEXT("GunName : %d"), Gun));
				switch (Gun) // Grenade = Pistol 모션 , Vehicle = Rifle-Long모션
				{
				case EGunName::AK47:
				case EGunName::M16:
				case EGunName::M24:
					InventoryCharacter->SetPoses(EPlayerActionMode::Rifle);
					break;
				case EGunName::AUG:
				case EGunName::GROZA:
				case EGunName::UZI:
				case EGunName::UMP:
				case EGunName::P18c:
				case EGunName::Sawedoff:
					InventoryCharacter->SetPoses(EPlayerActionMode::Grenade);
					break;
				case EGunName::VSS:
				case EGunName::M1897:
				case EGunName::S12K:
					InventoryCharacter->SetPoses(EPlayerActionMode::Vehicle);
					break;
				default:
					break;
				}
			}
		}
		Debug::Print("InventoryCharacter->SetPoses SUCCESS",FColor::Green);
	}
	else
	{
		Debug::Print("InventoryCharacter->SetPoses FAILED",FColor::Red);
	}
}

void APubgCharacter::OnRep_MaxWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = CurrentMaxWalkSpeed;
}

void APubgCharacter::OnRep_UpdateControllerRotationYaw()
{
	bUseControllerRotationYaw = bUpdateControllerRotationYaw;
}

void APubgCharacter::PlayerDead()
{
	Super::PlayerDead();
	
	Client_SummaryWidget(false,GetName());
}

void APubgCharacter::Client_SummaryWidget_Implementation(const bool bIs1st, const FString& OwnerName)
{
	APubgPlayerController* PlayerController= Cast<APubgPlayerController>(GetController());
	{
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->SetShowMouseCursor(true);
		GetHUDBase()->GetSummaryWidget()->SetVisibility(ESlateVisibility::Visible);
		GetHUDBase()->GetSummaryWidget()->Setup(bIs1st,OwnerName);
	}
}


void APubgCharacter::GetPlayerMoveDirection()
{
	FVector ForwardVector = GetActorForwardVector();
	FVector PlayerDirection = GetVelocity().GetSafeNormal();

	PlayerMoveDirection = FVector::DotProduct(ForwardVector, PlayerDirection);
}


void APubgCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APubgCharacter, PlayerBodyMode);
	DOREPLIFETIME(APubgCharacter, PlayerEquipMode);
	DOREPLIFETIME(APubgCharacter, bHasJump);
	DOREPLIFETIME(APubgCharacter, bIsLanding);
	DOREPLIFETIME(APubgCharacter, bIsZoomIn);
	DOREPLIFETIME(APubgCharacter, bHasTurn);
	DOREPLIFETIME(APubgCharacter, mIsLeaning);
	DOREPLIFETIME(APubgCharacter, NearActor);
	DOREPLIFETIME(APubgCharacter, bIsAttachedToPlane); 
	DOREPLIFETIME(APubgCharacter, AttachedSocketTransform);
	DOREPLIFETIME(APubgCharacter, bIsInventoryOpen);
	DOREPLIFETIME(APubgCharacter, bIsFreeFalling);
	DOREPLIFETIME(APubgCharacter, bIsFreeFallingLock);
	DOREPLIFETIME(APubgCharacter, FPP_MODE);
	DOREPLIFETIME_CONDITION(APubgCharacter, CurrentMaxWalkSpeed, COND_AutonomousOnly);
	DOREPLIFETIME(APubgCharacter, bUpdateControllerRotationYaw);
	
}
// 수정 테스트 무기별 블랜드 위해
void APubgCharacter::ChangePlayerGunType_Implementation(const EGunName& NewGunType)
{
	if (PubgAnimInstance)
	{
		PubgAnimInstance->GunType = NewGunType;
	}
}

void APubgCharacter::LimitCameraAngle(const EPlayerActionMode NewPlayerBodyMode)
{
	if (APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		switch (NewPlayerBodyMode)
		{
		case EPlayerActionMode::Prone:
			PlayerCameraManager->ViewPitchMin = -30.f;
			PlayerCameraManager->ViewPitchMax = 30.f;
			break;
		default:
			PlayerCameraManager->ViewPitchMin = -80.f;
			PlayerCameraManager->ViewPitchMax = 70.f;
			break;
		}
	}
}

void APubgCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	const float FallVelocity = GetVelocity().Z;
	if (FallVelocity <= FallHeight)
	{
		if (BaseAbilitySystemComponent && FallDamageEffect)
		{
			BaseAbilitySystemComponent->ApplyGameplayEffectToSelf(
				FallDamageEffect.GetDefaultObject(),
				1.f,
				BaseAbilitySystemComponent->MakeEffectContext()
			);
		}
	}
}


void APubgCharacter::FlipZoomIn()
{
	if (PlayerEquipMode == EPlayerActionMode::Unarmed) return;
	if (bIsFreeFalling) return;  // 낙하산 또는 자유낙하 시 줌 제한
	
	if (!bIsZoomIn)
	{
		Server_SetZoom_Implementation(true);
		bIsZoomIn = true;

		switch (PlayerEquipMode)
		{
		case EPlayerActionMode::Rifle:
			OnChangeCameraLocation(GetPoseCameraLocation(), GetTargetArmLength());
			break;
		default:
			break;
		}
	}
	if (!bUpdateControllerRotationYaw)
	{
		Server_ActiveControllerYaw();
	}
	bUseControllerRotationYaw = true;
}

void APubgCharacter::FlipZoomIn_Release()
{
	Server_SetZoom_Implementation(false);
	OnChangeCameraLocation(GetPoseCameraLocation(), GetTargetArmLength());
	Input_LCtrl_Release();
}




void APubgCharacter::OnZoomButtonPressed(const FInputActionInstance& ActionInstance)
{
	// 버튼이 눌린 시점 기록
	if (bIsFreeFalling) return; 
	bIsButtonPressed = true;
}

void APubgCharacter::OnZoomButtonReleased(const FInputActionInstance& ActionInstance)
{
	if (bIsButtonPressed)
	{
		if (ActionInstance.GetElapsedTime() < ShortPressThreshold)
		{
			FPP_Zoom();
			
		}
		// 상태 초기화
		bIsButtonPressed = false;
	}
}

void APubgCharacter::FPP_Zoom()
{
	// 엎드려 있을때 아래를 보고 있으면 줌 불가
	if (GetPlayerBodyMode() == EPlayerActionMode::Prone)
	{
		UPubgAnimInstance* AnimInstance = Cast<UPubgAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance->Pitch < -5.f) return;
	};
	if (ABaseGun* Gun = Cast<ABaseGun>(EquipManageComponent->GetCurrentWeapon()))
	{
		if (bFlipFlopState)
		{
			SetFPP_MODE(true);
			SetFPP_MODE_Client(true);
    	
			FAttachmentTransformRules AttachmentRules(
		   EAttachmentRule::SnapToTarget, // 위치 유지 규칙
		   EAttachmentRule::SnapToTarget, // 회전 유지 규칙
		   EAttachmentRule::KeepRelative,
		   true);
			float Fov = 0;
			const FName& Socket = Gun->GetGunSocketZoomInfo(Fov);
			FPP_Camera->SetFieldOfView(Fov);
			// Debug::Print(Socket.ToString());
			FPP_Camera->AttachToComponent(Gun->GetGunWeaponMesh(),AttachmentRules,Socket);
			FollowCamera->SetActive(false,false);
			FPP_Camera->SetActive(true,false);
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
			SetMouseSensitivity(Fov/90.f, Fov/90.f);
		}
		else
		{
			SetFPP_MODE(false);
			SetFPP_MODE_Client(false);
			FollowCamera->SetActive(true,false);
			FPP_Camera->SetActive(false,false);
			Gun->GunZoomOutReset();
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			SetMouseSensitivity(1.0f, 1.0f);
		}
	}
    // 상태 전환
    bFlipFlopState = !bFlipFlopState;
}

void APubgCharacter::Input_MoveCheck(const FInputActionValue& InputValue)
{
	if (GetPlayerBodyMode() == EPlayerActionMode::Prone && FPP_MODE == true)
	{
		FPP_Zoom();
	}
}

void APubgCharacter::Input_LookCheck(const FInputActionValue& InputValue)
{
	if (GetPlayerBodyMode() == EPlayerActionMode::Prone && FPP_MODE == true)
	{
		UPubgAnimInstance* AnimInstance = Cast<UPubgAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance->Pitch <= -2.f)
		{
			FPP_Zoom();
		}
	}
}




void APubgCharacter::ToggleInventory()
{
	if (GetHUDBase())
	{
		PlayerHUDBase->ToggleInventory();
		InventoryCharacter->SetOpenInventory(PlayerHUDBase->GetInventoryOpen());
		PlayerHUDBase->GetInventoryMainWidget()->GetGearWidget()->SetRenderTarget(InventoryCharacter->GetRenderTarget());
		InventoryCharacter->SetCurrentWeapon(GetEquipManageComponent()->GetCurrentWeaponSlotTag());
	}
}
void APubgCharacter::SetupPickupMontage_Implementation()
{
	PickupMontage = PickupMontageMap[PlayerEquipMode].MontageMap[PlayerBodyMode].Montages[NearActor.PickMode].Montages[0] ?PickupMontageMap[PlayerEquipMode].MontageMap[PlayerBodyMode].Montages[NearActor.PickMode].Montages[0] : nullptr;
}

void APubgCharacter::PickupCompleted()
{
	if (bIsPickUp)
		bIsPickUp = false;
}