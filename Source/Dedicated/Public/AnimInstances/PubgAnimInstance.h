// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/CharacterAnimInstance.h"
#include "BaseType/BaseEnumType.h"
#include "PubgAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EFallingType : uint8
{
	Normal = 0,
	Step_One,
	Step_Two,
	Step_Three
};
/**
 * 
 */
UCLASS()
class DEDICATED_API UPubgAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="ActionMode")
	EPlayerActionMode PlayerBodyMode = EPlayerActionMode::Stand;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="ActionMode")
	EPlayerActionMode PlayerEquipMode = EPlayerActionMode::Unarmed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Falling")
	EFallingType FallingType = EFallingType::Normal;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Falling")
	float FallingDuration;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Status")
	bool bIsFalling;

	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Status")
	float Pitch;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Status")
	float Yaw;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Status")
	FVector Velocity;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Status")
	bool bMoveJump;
	
	// Diagonal Turn when not Turn in place;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Status")
	bool bHasTurn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	bool bAming = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	bool bSprintProne;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Status")
	float mLeaning;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="ActionMode")
	EGunName GunType = EGunName::Default;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="FreeFalling")
	bool bFreeFalling = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="FreeFalling")
	bool bParachuteExit = true;
	
	// Turn Angle Check
	UFUNCTION(BlueprintCallable, Category="Turn")
	void CheckTurnDirection();

	//Falling Time Check Function
	void CheckFallingState(const float _DeltaSeconds);


	UFUNCTION()
	void SetCharacterControlYaw();

	UFUNCTION()
	void SetMoveJump(bool _Bool);
	
	FORCEINLINE EFallingType GetFallingType() const { return FallingType; }
	FORCEINLINE EFallingType SetFallingType(const EFallingType _FallingType) { return FallingType = _FallingType; }
	FORCEINLINE void SetActiveFreeFalling() {bFreeFalling = true;}
	FORCEINLINE void ActiveParachuteExit() {bParachuteExit = false;}
	FORCEINLINE bool GetParachuteExit() {return bParachuteExit;}
	

	
	
};
