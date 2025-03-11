// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BaseInteractComponent.h"

#include "Characters/PubgCharacter.h"
#include "Components/BasePickUpComponent.h"
#include "Components/SphereComponent.h"
#include "Components/Renewal/PickUpManageComponent.h"
#include "DSP/AudioDebuggingUtilities.h"
#include "FunctionLibrary/BaseFunctionLibrary.h"
#include "HUD/HUDBase.h"
#include "HUD/UI/InventoryMainWidget.h"
#include "Item/BaseFieldItem.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"
#include "Subsystem/PubgDataTableManageSubsystem.h"


// Sets default values for this component's properties
UBaseInteractComponent::UBaseInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->InitSphereRadius(Radius);
	SphereComp->SetHiddenInGame(false);
	

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> OutlineAsset(TEXT("/Game/Asset/Material/MI/MI_Outline.MI_Outline"));
	if (OutlineAsset.Succeeded())
	{
		MI_Outline = OutlineAsset.Object;
	}
	
}

void UBaseInteractComponent::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	//플레이어의 PickupComponent의 Candidate에 추가 및 InventoryUI에 해당 아이템 정보 현행화.
	if (APubgCharacter* Character = Cast<APubgCharacter>(OtherActor))
	{
		if (Character->IsLocallyControlled())
		{
			if (ABaseFieldItem* FieldItem = GetOwner<ABaseFieldItem>())
			{
				if (UPickUpManageComponent* PickUpComponent = Character->GetPickUpManageComponent())
				{
					PickUpComponent->AddPickUpCandidate(FieldItem);
					Character->GetHUDBase()->GetInventoryMainWidget()->UpdateAroundItem();
				}
			}
		}


		// 아이템 Blink 효과 활성화
		if (MaterialInstance)
		{
			MaterialInstance->SetScalarParameterValue(TEXT("Blink"), 1);
			Debug::Print("StaticMeshComponent :: set Material");
		}
		
	}
	
}

void UBaseInteractComponent::SphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//플레이어의 PickupComponent의 Candidate에 자기 자신 삭제 및 InventoryUI에 해당 아이템 정보 현행화.
	if (APubgCharacter* Character = Cast<APubgCharacter>(OtherActor))
	{
		if (Character->IsLocallyControlled())
		{
			if (ABaseFieldItem* FieldItem = Cast<ABaseFieldItem>(GetOwner()))
			{
				if (UPickUpManageComponent* PickUpComponent = Character->GetPickUpManageComponent())
				{
					PickUpComponent->RemovePickUpCandidate(FieldItem);
					Character->GetHUDBase()->GetInventoryMainWidget()->UpdateAroundItem();
				}
			}
		}

		// 관련 정보 초기화
		if (UStaticMeshComponent* StaticMeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>())
		{
			StaticMeshComponent->SetOverlayMaterial(nullptr);
			Character->InitCloseActor();
			bOnMaterial = false;
			//Debug::Print("Call EndOverlap");
		}


		// Blink 효과 비활성화
		if (MaterialInstance)
		{
			MaterialInstance->SetScalarParameterValue(TEXT("Blink"), 0);
		}
		
		
	}
}

void UBaseInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Owner의 RootComponent에 SphereComp 부착
	if (GetOwner())
		SphereComp->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	if (GetNetMode() == NM_Client) // 클라이언트에서만 overlap 처리
	{
		SphereComp->OnComponentBeginOverlap.AddDynamic(this,&UBaseInteractComponent::SphereBeginOverlap);
		SphereComp->OnComponentEndOverlap.AddDynamic(this,&UBaseInteractComponent::SphereEndOverlap);

		
		if (ABaseFieldItem* FieldItem = Cast<ABaseFieldItem>(GetOwner()))
		{
			ItemID = FieldItem->GetItemID();

			if (const FInventoryDataStruct* ItemData = UBaseFunctionLibrary::GetItemData(ItemID))
			{
				ItemName = ItemData->DisplayName;
			}
		}
	}

	// Owner의 StaticMesh에 설정된 Material Instance 가져오기
	if (UStaticMeshComponent* StaticMeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>())
	{
		// 설정된 머티리얼 가져오기
		UMaterialInterface* Material = StaticMeshComponent->GetMaterial(0);

		if (Material)
		{
			// MI인지 확인
			UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);

			// MI가 아니라면 생성 후 설정
			if (!DynamicMaterial)
			{
				DynamicMaterial = UMaterialInstanceDynamic::Create(Material, nullptr);
				StaticMeshComponent->SetMaterial(0, DynamicMaterial);
			}

			// 캐싱
			MaterialInstance = DynamicMaterial;
		}
	}
	
	checkf(MI_Outline, TEXT("Fail Load Outline.Material"));
}


void UBaseInteractComponent::ToggleOverlayMaterial(const bool Toggle)
{
	if (bOnMaterial == Toggle) return;
	
	if (UStaticMeshComponent* StaticMeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>())
	{
		if (!Toggle)
			StaticMeshComponent->SetOverlayMaterial(nullptr);
		else
			StaticMeshComponent->SetOverlayMaterial(MI_Outline);
		
		bOnMaterial = Toggle;
	}
}

