// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PubgInventoryCharacter.h"

#include "BaseDebugHelper.h"
#include "BaseGameplayTags.h"
#include "AnimInstances/InventoryCharacterAnimInstance.h"
#include "Characters/PubgCharacter.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/Renewal/EquipManageComponent.h"
#include "Engine/TextureRenderTarget2D.h" 
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "Gear/BaseGear.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"
#include "Item/BaseGearFieldItem.h"
#include "Weapon/BaseGun.h"


// Sets default values
APubgInventoryCharacter::APubgInventoryCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->SetupAttachment(RootComponent);
	PrimaryMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PrimaryMesh"));
	PrimaryMesh->SetupAttachment(SkeletalMeshComponent);
	SecondaryMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SecondaryMesh"));
	SecondaryMesh->SetupAttachment(SkeletalMeshComponent);
	CurrentWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CurrentWeaponMesh"));
	SecondaryMesh->SetupAttachment(SkeletalMeshComponent);
	

	

	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Asset/Character/Female/Mesh/Female.Female"));
	if (MeshAsset.Succeeded())
	{
		SkeletalMeshComponent->SetSkeletalMesh(MeshAsset.Object);
	}
	
	static ConstructorHelpers::FClassFinder<UInventoryCharacterAnimInstance> AnimBPClass(TEXT("/Game/_BP/Character/Inventory/ABP_Inventory_Character.ABP_Inventory_Character_C"));
	if (AnimBPClass.Succeeded())
	{
		SkeletalMeshComponent->SetAnimInstanceClass(AnimBPClass.Class);
	}

	UniqueRenderTarget = NewObject<UTextureRenderTarget2D>();
	if (UniqueRenderTarget)
	{
		UniqueRenderTarget->InitAutoFormat(430*2,980*2);
		SceneCaptureComponent->TextureTarget = UniqueRenderTarget;
	}

	
	SkeletalMeshComponent->SetRelativeRotation(FRotator(-1.257466f, 7.097812f, 10.185986f));
	SceneCaptureComponent->SetRelativeLocation(FVector(-6.737232f, 288.617139f, 89.257796f));
	SceneCaptureComponent->SetRelativeRotation(FRotator(0.f, -90.000000f, 0.f));
	SceneCaptureComponent->FOVAngle = 20.f;
	SceneCaptureComponent->bCaptureEveryFrame = false;
	SceneCaptureComponent->bCaptureOnMovement = false;
	
	PrimaryMesh->SetHiddenInGame(true);
	SecondaryMesh->SetHiddenInGame(true);


	// Costume
	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	BodyMesh -> SetupAttachment(SkeletalMeshComponent);
	BodyMesh->SetLeaderPoseComponent(SkeletalMeshComponent);

	LegMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegMesh"));
	LegMesh-> SetupAttachment(SkeletalMeshComponent);
	LegMesh->SetLeaderPoseComponent(SkeletalMeshComponent);

	FeetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FeetMesh"));
	FeetMesh-> SetupAttachment(SkeletalMeshComponent);
	FeetMesh->SetLeaderPoseComponent(SkeletalMeshComponent);
	
	HelmetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HelmetMesh"));
	HelmetMesh -> SetupAttachment(SkeletalMeshComponent);
	HelmetMesh->SetLeaderPoseComponent(SkeletalMeshComponent);

	BackpackMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BackpackMesh"));
	BackpackMesh-> SetupAttachment(SkeletalMeshComponent);
	BackpackMesh->SetLeaderPoseComponent(SkeletalMeshComponent);

	VestMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VestMesh"));
	VestMesh-> SetupAttachment(SkeletalMeshComponent);
	VestMesh->SetLeaderPoseComponent(SkeletalMeshComponent);
}

UInventoryCharacterAnimInstance* APubgInventoryCharacter::GetAnimInstance()
{
	if (CharacterAnimInstance == nullptr)
	{
		CharacterAnimInstance = Cast<UInventoryCharacterAnimInstance>(SkeletalMeshComponent->GetAnimInstance());
		return CharacterAnimInstance;
	}

	return CharacterAnimInstance;
}

void APubgInventoryCharacter::SetPoses(EPlayerActionMode NewPoses)
{
	if (GetAnimInstance()->GetEquipMode() != EPlayerActionMode::Unarmed)
		GetAnimInstance()->SetEquipMode(EPlayerActionMode::Unarmed);
	Debug::Print("Change Unarmed");
	
	FTimerHandle TimerHandle_Equip;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Equip, [this, NewPoses]()
	{
		GetAnimInstance()->SetEquipMode(NewPoses);
		Debug::Print("Success Change");
	},0.2f,false
	);
	
}

void APubgInventoryCharacter::UpdateGear()
{
	// Owner 가져오기
	UEquipManageComponent* EquipManager = OwnerPawn->GetEquipManageComponent();
	if (!EquipManager)
	{
		Debug::Print("APubgInventoryCharacter :: Equip Manager not found");
		return;
	}

	// 반복문 돌리기 위해 Gear Tag Array 생성
	TArray<FGameplayTag> GearTags = {
		BaseGameplayTags::Item_Gear_Helmet,
		BaseGameplayTags::Item_Gear_Armor,
		BaseGameplayTags::Item_Gear_Backpack
	};

	// Tag 관련 메쉬 매핑
	TMap<FGameplayTag, USkeletalMeshComponent*> GearMeshMap = {
		{BaseGameplayTags::Item_Gear_Helmet, HelmetMesh},
		{BaseGameplayTags::Item_Gear_Armor, VestMesh},
		{BaseGameplayTags::Item_Gear_Backpack, BackpackMesh},
	};
	

	for (const FGameplayTag& Tag : GearTags)
	{
		if (USkeletalMeshComponent** MeshCompPtr = GearMeshMap.Find(Tag))
		{
			USkeletalMeshComponent* MeshComp = *MeshCompPtr;
			if (EquipManager->GetGearInSlot(Tag))
				MeshComp->SetSkeletalMesh(EquipManager->GetGearInSlot(Tag)->GetGearMesh()->GetSkeletalMeshAsset());
			else
				MeshComp->SetSkeletalMesh(nullptr);
		}
	}
}

void APubgInventoryCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PrimaryMesh->AttachToComponent(SkeletalMeshComponent,FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("slot_primary"));
	SecondaryMesh->AttachToComponent(SkeletalMeshComponent,FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("slot_secondary"));
	CurrentWeaponMesh->AttachToComponent(SkeletalMeshComponent,FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("item_r"));
	SecondaryMesh->SetRelativeLocationAndRotation(FVector(0.f,-14.f,5.f),FRotator(0.f,25.f,90.f));
	PrimaryMesh->SetRelativeLocationAndRotation(FVector(-12.f,-2.f,5.f),FRotator(5.f,40.f,-90.f));
}

// Called when the game starts or when spawned
void APubgInventoryCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!CharacterAnimInstance)
	{
		CharacterAnimInstance = Cast<UInventoryCharacterAnimInstance>(SkeletalMeshComponent->GetAnimInstance());
	}

	SceneCaptureComponent->ShowOnlyActorComponents(this);
}

void APubgInventoryCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOpenInventory)
	{
		SceneCaptureComponent->CaptureScene();
	}
}

void APubgInventoryCharacter::SetPrimaryWeapon(const int32 _ItemID)
{
	if (_ItemID != 0)
	{
		PrimaryMesh->SetSkeletalMesh(GetWeapon(_ItemID));
		PrimaryMesh->SetHiddenInGame(false);
	}
	else
	{
		PrimaryMesh->SetHiddenInGame(true);
	}
	
}

void APubgInventoryCharacter::SetSecondaryWeapon(const int32 _ItemID)
{
	if (_ItemID != 0)
	{
		SecondaryMesh->SetSkeletalMesh(GetWeapon(_ItemID));
		SecondaryMesh->SetHiddenInGame(false);
	}
	else
	{
		SecondaryMesh->SetHiddenInGame(true);
	}
}

void APubgInventoryCharacter::SetCurrentWeapon(const FGameplayTag& _WeaponTag)
{
	if (!this) return;


	
	if (_WeaponTag.MatchesTag(BaseGameplayTags::Item_Slot_Primary))
	{
		PrimaryMesh->SetHiddenInGame(true);
		SecondaryMesh->SetHiddenInGame(false);
		CurrentWeaponMesh->SetHiddenInGame(false);
		CurrentWeaponMesh->SetSkeletalMesh(PrimaryMesh->GetSkeletalMeshAsset());
	}
	else if (_WeaponTag.MatchesTag(BaseGameplayTags::Item_Slot_Secondary))
	{
		SecondaryMesh->SetHiddenInGame(true);
		PrimaryMesh->SetHiddenInGame(false);
		CurrentWeaponMesh->SetHiddenInGame(false);
		CurrentWeaponMesh->SetSkeletalMesh(SecondaryMesh->GetSkeletalMeshAsset());
	}
	else // 추후 권총도 추가해야할듯.
	{
		CurrentWeaponMesh->SetHiddenInGame(true);
		Debug::Print("InventoryCharacter:: SetCurrentWeapon is Failed");
	}
}

USkeletalMesh* APubgInventoryCharacter::GetWeapon(const int32 _Item)
{
	return WeaponArray.Find(_Item) ? *WeaponArray.Find(_Item) : nullptr;
}
