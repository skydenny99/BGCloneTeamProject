// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnumType.h"
#include "BaseGameplayTags.h"
#include "GameplayTagContainer.h"
#include "BaseStructType.generated.h"


class UGameplayEffect;

USTRUCT(BlueprintType)
struct FGunSpec : public FTableRowBase
{
	GENERATED_BODY()
	// 총기 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Spec")
	int32 ItemID = 0;
	
	// 총기 종류
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Spec")
	EGunType GunType = EGunType::AR;

	// 총기 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Spec")
	EGunName GunName = EGunName::Default;
	
	// 기본 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Spec")
	float Damage = 5;
	// Moa
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Spec")
	float Moa = 3;
	// 총기 최대 탄약
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Spec")
	int32 MaxBulletCount = 30;
	// 지원하는 발사모드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Spec")
	FGameplayTagContainer FireModes;
	
	// 이동속도 페널티
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Spec")
	TSubclassOf<UGameplayEffect> SpeedPenalty;
	// 수평 반동
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Spec")
	float HorizontalRecoil;
	// 수직 반동
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Spec")
	float VerticalRecoil;
	// 연사 간격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Spec")
	TMap<FGameplayTag, float> FireRate =
	{
		{BaseGameplayTags::Weapon_Gun_FireMode_Single, 0.1f},
		{BaseGameplayTags::Weapon_Gun_FireMode_Burst, 0.1f},
		{BaseGameplayTags::Weapon_Gun_FireMode_Auto, 0.1f}
	};
	// 점사 간격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Spec")
	float BurstFireRate = 0.1f;
	// 사용 탄약
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Spec")
	EBulletType UsedBullet = EBulletType::B_5_56mm;
	// 탄속 ( m/s )
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Spec")
	float BulletSpeed = 800.f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Spec")
	UCurveFloat* BulletDamageCurve = nullptr;
	// 파츠에 따른 스펙 (추후 별도 추가
	

	/**HUD에서 현재 장착 이미지*/
	UPROPERTY(EditAnywhere, Category = "Description")
	UTexture2D* HUDWeaponImage;
};


USTRUCT(BlueprintType)
struct FGunDamageSpecInfo
{
	GENERATED_BODY();
	UPROPERTY()
	float BaseDamage;
	UPROPERTY()
	EGunType GunType;
	UPROPERTY()
	float FireRangeDamageMultiplier;
	
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
};
// FORCEINLINE FArchive &operator<<(FArchive &Ar, EGunType& TheEnum)
// { 
// 	uint8 AsByte = static_cast<uint8>(TheEnum); 	//When saving
//
// 	//Serialize as byte
// 	Ar << AsByte;			
// 	
// 	//Loading
// 	if(Ar.IsLoading())
// 	{
// 		TheEnum = static_cast<EGunType>(AsByte); //When loading
// 	} 
// 	 
// 	return Ar;
// }

// template<>
// struct TStructOpsTypeTraits<FGunDamageSpecInfo> : public TStructOpsTypeTraitsBase2<FGunDamageSpecInfo>
// {
// 	enum
// 	{
// 		WithNetSerializer = true
// 	};
// };



USTRUCT(BlueprintType)
struct FPickupMontageList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> Montages;
};

USTRUCT(BlueprintType)
struct FPickupMontageData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EPickMode, FPickupMontageList> Montages;
};

USTRUCT(BlueprintType)
struct FPickupMontageCollection
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EPlayerActionMode, FPickupMontageData> MontageMap; 
};

USTRUCT(BlueprintType)
struct FNearActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	AActor* NearActor  = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	AActor* BeforeActor  = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	EPickMode PickMode = EPickMode::Low;
};