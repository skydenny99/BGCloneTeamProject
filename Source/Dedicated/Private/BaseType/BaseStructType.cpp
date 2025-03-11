// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseType/BaseStructType.h"


bool FGunDamageSpecInfo::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	bool bNoBaseDamage = false;
	bool bNoGunTypeDamageMultiplier = false;
	bool bNoFireRangeDamageMultiplier = false;

	if (Ar.IsSaving())
	{
		bNoBaseDamage = (BaseDamage == 0);
		bNoGunTypeDamageMultiplier = (GunType == EGunType::None);
		bNoFireRangeDamageMultiplier = (FireRangeDamageMultiplier == 0);
	}
	uint8 Flags = (bNoBaseDamage << 0) | (bNoGunTypeDamageMultiplier << 1) | (bNoFireRangeDamageMultiplier << 2) ;
	bNoBaseDamage = (Flags & (1 << 0)) ? 1 : 0;
	bNoGunTypeDamageMultiplier = (Flags & (1 << 1)) ? 1 : 0;
	bNoFireRangeDamageMultiplier = (Flags & (1 << 2)) ? 1 : 0;
	
	if (bNoBaseDamage == false)
	{
		Ar << BaseDamage;
	}

	// if (bNoGunTypeDamageMultiplier == false)
	// {
	// 	Ar << GunType;
	// }

	if (bNoFireRangeDamageMultiplier == false)
	{
		Ar << FireRangeDamageMultiplier;
	}

	return true;
}
