// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PubgDataTableManageSubsystem.generated.h"

struct FGunSpec;
struct FInventoryDataStruct;
/**
 * 
 */
UCLASS()
class DEDICATED_API UPubgDataTableManageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	

private:
	UPROPERTY(Transient)
	TMap<FString, UDataTable*> DataTables;

	static UPubgDataTableManageSubsystem* Instance;
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	template<typename T>
	const T* FindRow(const FName& RowName);
	
	template<typename T>
	const T* FindRow(int32 IntRowName);

	static UPubgDataTableManageSubsystem* GetInstance() { return Instance; }
};

template <typename T>
const T* UPubgDataTableManageSubsystem::FindRow(const FName& RowName)
{
	UDataTable* DataTable;
	const FString TableName = T::StaticStruct()->GetPathName();
	if (DataTables.Contains(TableName))
	{
		DataTable = DataTables[TableName];
	}
	else
	{
		return nullptr;
	}

	const T* Result = DataTable->FindRow<T>(RowName, TEXT(""));
	return Result;
}

template <typename T>
const T* UPubgDataTableManageSubsystem::FindRow(int32 IntRowName)
{
	return UPubgDataTableManageSubsystem::FindRow<T>(FName(FString::FromInt(IntRowName)));
}
