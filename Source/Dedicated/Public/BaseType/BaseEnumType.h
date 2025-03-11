#pragma once


UENUM(BlueprintType)
enum class EBulletType : uint8
{
	None = 0,
	B_5_56mm = 150  UMETA(DisplayName = "5.56mm") ,
	B_7_62mm = 151 UMETA(DisplayName = "7.62mm"),
	B_9mm = 152 UMETA(DisplayName = "9mm"),
	B_12gauge = 153 UMETA(DisplayName = "12 Gauge")
};

//Player 행동 Enum
UENUM(BlueprintType)
enum class EPlayerActionMode : uint8
{
	Stand = 0,
	Crouch,
	Prone,
	Vehicle,
	Unarmed,
	Rifle,
	Melee,
	Grenade
};


UENUM(BlueprintType)
enum class EGunType : uint8
{
	None,
	AR,
	LMG,
	DMR,
	SR,
	SG,
	SMG,
	Pistol
};

UENUM(Blueprintable)
enum class EGunName : uint8
{
	Default = 0,
	AK47,
	AUG,
	M16,
	GROZA,
	VSS,
	UZI,
	UMP,
	M24,
	M1897,
	S12K,
	P18c,
	Sawedoff	
};

UENUM(BlueprintType)
enum class EPickMode : uint8
{
	Low = 0 UMETA(DisplayName = "Low"),
	Middle UMETA(DisplayName = "Middle"),
	High UMETA(DisplayName = "High")
};