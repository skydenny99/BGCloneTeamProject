// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/PubgDataTableManageSubsystem.h"

#include "BaseDebugHelper.h"


UPubgDataTableManageSubsystem* UPubgDataTableManageSubsystem::Instance = nullptr;

void UPubgDataTableManageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	const static FString TableDirPath = TEXT("/Game/_BP/Datas/DataTable/");
	static TArray<FString> TableNames =
		{
		TEXT("InventoryDataTable"),
		TEXT("DT_GunSpec")
		};
	for (const auto& TableName : TableNames)
	{
		if (UDataTable* LoadedTable = LoadObject<UDataTable>(nullptr, *FString::Printf(TEXT("%s%s.%s"), *TableDirPath, *TableName, *TableName)))
		{
			DataTables.Add(LoadedTable->GetRowStruct()->GetStructPathName().ToString(), LoadedTable);
		}
	}
	Instance = this;
	Debug::Print("UPubgDataTableManageSubsystem::Initialize");
}

void UPubgDataTableManageSubsystem::Deinitialize()
{
	Super::Deinitialize();
	Instance = nullptr;
}
