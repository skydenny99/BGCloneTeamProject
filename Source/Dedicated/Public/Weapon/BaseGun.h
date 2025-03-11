// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseType/BaseStructType.h"
#include "Weapon/WeaponBase.h"
#include "Engine/StreamableManager.h"
#include "BaseGun.generated.h"

/**
 * 
 */


class AProjectileBase;
enum class EBulletType : uint8;
enum class EWeaponFireMode : uint8;



DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateCurrentAmmo, int32);


UCLASS()
class DEDICATED_API ABaseGun : public AWeaponBase
{
	GENERATED_BODY()
 
public:
	ABaseGun();
	
protected:
	UFUNCTION()
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category= "Weapon Stat")
	FString GunName;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category= "Weapon Stat")
	FGunSpec GunSpec;
	UPROPERTY(VisibleDefaultsOnly, ReplicatedUsing=OnRep_MaxAmmo, Category= "Weapon Stat");
	uint8 MaxAmmo;
	UPROPERTY(VisibleDefaultsOnly, ReplicatedUsing=OnRep_CurrentAmmo, Category= "Weapon Stat");
	uint8 CurrentAmmo;
	UPROPERTY(ReplicatedUsing=OnRep_FireMode)
	int32 FireModeIndex;

	UPROPERTY(VisibleDefaultsOnly, Replicated, Category= "Weapon Stat")
	float ReloadSpeedRate = 1.f;
	

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// UPROPERTY(Transient)
	// TMap<FGameplayTag, class UPubgGunFireBase*> FireModeStrategies;
	// UPROPERTY(Transient)
	// UPubgGunFireBase* CurrentFireModeStrategy;
	// void SetFireModeStrategy(const FGameplayTag& FireModeTag);
	// void BeginFire();
	// void StopFire();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* FireMontage = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* ReloadMontage = nullptr;
	
	
public:
	FOnUpdateCurrentAmmo OnUpdateCurrentAmmoDelegate;
	
	UFUNCTION(BlueprintPure)
	virtual bool CanAttack() const override;

	UFUNCTION(BlueprintPure)
	bool CanReload() const;
	virtual void OnPutAway() override;

	UFUNCTION(BlueprintPure)
	FGameplayTag GetCurrentFireMode() const;

	UFUNCTION(BlueprintCallable)
	void SwitchFireMode();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Effects")
	void FireWeaponEffect();

	UFUNCTION(Server, Reliable)
	void Server_AddAmmo(uint8 InAmount);

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulti_PlayFireMontage();
	
	UFUNCTION(NetMulticast, Unreliable)
	void NetMulti_PlayReloadMontage();
	
	// ZOOM info
	UFUNCTION()
	FName GetGunSocketZoomInfo(float& OutVal);
	
	// ZoomoutReset
	UFUNCTION()
	void GunZoomOutReset();


#pragma region Replication
	UFUNCTION()
	void OnRep_MaxAmmo();

	
	UFUNCTION()
	void OnRep_CurrentAmmo();

	UFUNCTION()
	void OnRep_FireMode();
#pragma endregion 

#pragma region getter & setters
	UFUNCTION(BlueprintPure)
	float GetFireRate() const;
	

	UFUNCTION(BlueprintPure, Category = "WeaponMesh")
	USkeletalMeshComponent* GetGunWeaponMesh() const {return WeaponMeshComp;}
	
	UFUNCTION(BlueprintPure)
	const FGunSpec& GetGunSpec() const {return  GunSpec;}
	
	UFUNCTION(BlueprintPure)
	uint8 GetCurrentAmmo() const {return CurrentAmmo;}
	
	UFUNCTION(BlueprintCallable)
	void SetCurrentAmmo(const int32 Value) {CurrentAmmo = Value;}

	UFUNCTION(BlueprintPure)
	uint8 GetMaxAmmo() const {return MaxAmmo;}

	UFUNCTION(BlueprintPure)
	float GetReloadSpeedRate() const { return ReloadSpeedRate; }

	FORCEINLINE void SetReloadSpeedRate(float InReloadRate) {ReloadSpeedRate = InReloadRate;}
	void IncreaseMaxAmmo(uint8 InAmount);
#pragma endregion
};




