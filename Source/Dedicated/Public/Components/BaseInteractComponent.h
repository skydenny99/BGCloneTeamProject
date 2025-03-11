// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"
#include "BaseInteractComponent.generated.h"


class USphereComponent;

UENUM(BlueprintType)
enum class EMaterialMode : uint8
{
	Blink = 0 UMETA(DisplayName = "Blink"),
	Outline UMETA(DisplayName = "Outline"),
	Outline_2 UMETA(DisplayName = "Outline_2")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEDICATED_API UBaseInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBaseInteractComponent();

#pragma region Status
private:
	/** 적용된 아이템 고유 아이디 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_InteractSetup", meta = (AllowPrivateAccess = "true"))
	int32 ItemID = -1;

	/** 적용된 아이템 DisplayName*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "A_InteractSetup", meta = (AllowPrivateAccess = "true"))
	FText ItemName;

	/** 주위 반경 Comp 사이즈 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_InteractSetup", meta = (AllowPrivateAccess = "true"))
	float Radius = 150.f;
	
	UPROPERTY(VisibleDefaultsOnly, Category = Interact, meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereComp;

	/** 현재 설정된 머티리얼 인스턴스를 담는 변수 */
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly, Category = "A_InteractSetup", meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* MaterialInstance;

	/** 물체 표면에 흰색라인 표시 */
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly, Category = "A_InteractSetup", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* MI_Outline;

	/** 현재 OutLine Material 활성화 여부 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bOnMaterial;
	
#pragma endregion

	UFUNCTION()
	void SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void SphereEndOverlap(UPrimitiveComponent*  OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	
	
protected:
	virtual void BeginPlay() override;

public:
	/** OutLine Material 스위치
	 *  @param Toggle On / Off 
	 */
	UFUNCTION()
	void ToggleOverlayMaterial(const bool Toggle);

	FORCEINLINE FText GetItemName() const { return ItemName; }
	FORCEINLINE bool GetOnMaterial() const { return bOnMaterial; }

};
