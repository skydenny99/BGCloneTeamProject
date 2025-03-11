// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/PlayerInventoryComponent.h"
#include "BaseDebugHelper.h"
#include "HUD/PlayerHUD_Child/PlayerStateSlot/BreathSlotWidget.h"
#include "InventorySystem/InventoryStructs/InventoryDataStruct.h"  // 변경된 경로

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // Disable Tick to optimize performance
	
	// If the DataTable is nullptr, set the default DataTable from the specified path
	//데이터 테이블이 nullptr인 경우 기본 경로의 데이터 테이블을 설정
	static ConstructorHelpers::FObjectFinder<UDataTable> DefaultDataTable(TEXT("/Game/_BP/Datas/DataTable/InventoryDataTable.InventoryDataTable"));
	if (DefaultDataTable.Succeeded())
	{
		ItemDataTable = DefaultDataTable.Object;
		// Debug::Print("Data table connection successful.",FColor::Blue);
	}
	else
	{
		Debug::Print("ItemDataTable not found. Address verification required. (Critical Error)!",FColor::Red);
		Debug::Print("Error called -> PlayerInventoryComponent.cpp(12) = InventoryDataTable Is Empty",FColor::Red);
	}
	//Start EquipWeight Weight 0.0f;
	UpdateWeight(0.0f);
	
	//CurrentBulletCount -> SetBulletDatas
	CurrentBulletCount.Add(EBulletType::B_5_56mm,0);
	CurrentBulletCount.Add(EBulletType::B_7_62mm,0);
	CurrentBulletCount.Add(EBulletType::B_9mm,0);
	CurrentBulletCount.Add(EBulletType::B_12gauge,0);
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Log an error and handle the exception if the DataTable is not set
	// 데이터 테이블이 설정되지 않은 경우 로그출력 예외 처리
	if (!ItemDataTable)
	{
		Debug::Print("ItemDataTable not found, returning False. (Critical Error)!",FColor::Red);
		Debug::Print("Error called -> PlayerInventoryComponent.cpp(33)",FColor::Red);
	}
	else
	{
		// Copy DataTableData -> Array=(InventoryItems, AroundItems)
		// Retrieve all rows from the DataTable : 데이터 테이블의 모든 행 검색
		for (const TPair<FName, uint8*>& Row : ItemDataTable->GetRowMap())
		{
			// Cast row data to a struct : 행 데이터를 구조체로 캐스트
			if (const FInventoryDataStruct* DataTable = reinterpret_cast<FInventoryDataStruct*>(Row.Value))
			{
				FInventoryItems CombinedItem;
				// 데이터 테이블 에서 정보 전달 받기
				// 아이템 ID
				CombinedItem.ItemID = DataTable->ItemID;
				// 아이템 Type
				CombinedItem.ItemType = DataTable->ItemType;
				// 아이템 Weight
				CombinedItem.ItemWeight = DataTable->ItemWeight;
				// Item Default Count : 아이템 기본값 0;
				CombinedItem.ItemCount = 0; 

				// Add to the InventoryItems array : InventoryItems 반복역에 추가
				InventoryItems.Add(CombinedItem);

				// Debug log : 디바그 로그
			}
		}
		//Debug::Print("Added item to arrays: ",FColor::Blue);

		// 테스트 초기 불릿 추가
		int32 OutVal;
		AcquireItem(151,30, OutVal);
	}
	//remove
}


// Local function F : 로컬 함수F

// Perform inventory weight capacity update when unequipping gear
// 장비를 장착 해제 했을때 인벤토리 보관 가능 무게증가 감수 업데이트 수행

void UPlayerInventoryComponent::UpdateWeight(float InEquipWeight)
{
	EquipWeight = InEquipWeight;
	MaxWeight = DefualtWeight+EquipWeight;

	//Debug
	// DebugText = FString::Printf(TEXT("Updated MaxWeight Weight: %f / %f + %f"), MaxWeight, DefualtWeight,EquipWeight);
	// Debug::Print(DebugText,FColor::Blue);
}

void UPlayerInventoryComponent::AddUISlot(int32 InItemID,  int32 InItemCount)
{
	const FInventoryDataStruct* CheckDataTable = ItemDataTable->FindRow<FInventoryDataStruct>(FName(*FString::FromInt(InItemID)), TEXT(""));
	
	FInventoryUI AddItem;
	AddItem.ItemID = CheckDataTable->ItemID;
	AddItem.ItemDisplayName = CheckDataTable->ItemDisplayName;
	AddItem.ItemDescription = CheckDataTable->ItemDescription;
	AddItem.ItemWeight = CheckDataTable->ItemWeight;
	AddItem.ItemIconPath = CheckDataTable->ItemIconPath;
	AddItem.ItemCount = InItemCount;
	AddItem.ItemIcon = CheckDataTable->ItemIcon;
	InventoryUI.Add(AddItem);
}

void UPlayerInventoryComponent::AcquireDropItem(FInventoryItems& ItemList, int32 InCount, int32 RemainingCount ,int32& OutCount)
{
	// 아이템을 전부 담을수 있을때 
	if (CurrentWeight + (InCount*ItemList.ItemWeight) <= MaxWeight)
	{
		//Debug::Print("Is Not Bullet",FColor::Blue);
						
		//DebugText = FString::Printf(TEXT("The item was found in the Itemlist. InItemID %d Count %d"),ItemList.ItemID,ItemList.ItemCount);
		//Debug::Print(DebugText,FColor::Blue);
		ItemList.ItemCount += InCount;
		
		//DebugText = FString::Printf(TEXT("The Itemlist Update. InItemID %d Count %d  ,CurrentWeight %f"),ItemList.ItemID,ItemList.ItemCount,CurrentWeight);
		//Debug::Print(DebugText,FColor::Blue);
						
		CurrentWeight += InCount*ItemList.ItemWeight;
						
		//DebugText = FString::Printf(TEXT("Updated Weight:ItemWeight %f = Update Weight %f / %f "),InCount*ItemList.ItemWeight,CurrentWeight,MaxWeight);
		//Debug::Print(DebugText,FColor::Blue);
						
		OutCount = 0;	
	}
	// 무게가 초과 되었을때
	else
	{
		int32 MaxAddableCount = (MaxWeight - CurrentWeight) / ItemList.ItemWeight;
		//Debug::Print("Is Not Bullet",FColor::Blue);
						
		//DebugText = FString::Printf(TEXT("The item was found in the Itemlist. InItemID %d"),ItemList.ItemID);
		//Debug::Print(DebugText,FColor::Blue);
						
		ItemList.ItemCount += MaxAddableCount; 
						
		RemainingCount -= MaxAddableCount;
						
		OutCount = RemainingCount;
		//Debug::Print("The item was found in the Itemlist but not fully added");
		//DebugText = FString::Printf(TEXT("The Itemlist Update. InItemID %d Count %dRemainItem %d,CurrentWeight %f"),ItemList.ItemID,MaxAddableCount,OutCount,CurrentWeight);
		//Debug::Print(DebugText,FColor::Blue);
		CurrentWeight += MaxAddableCount * ItemList.ItemWeight;
		//DebugText = FString::Printf(TEXT("Updated Weight:, ItemWeight %f = Update Weight %f / %f "),MaxAddableCount*ItemList.ItemWeight,CurrentWeight,MaxWeight);
		//Debug::Print(DebugText,FColor::Blue);
	}
}

void UPlayerInventoryComponent::AcquireDropAmmo(FInventoryItems& ItemList, int32 InCount, int32 RemainingCount ,int32& OutCount)
{
	// 아이템을 전부 담을수 있을때 
	DebugText = FString::Printf(TEXT("Current Weight %f, max Weight: %f "),CurrentWeight,MaxWeight);
	Debug::Print(DebugText,FColor::Blue);
	if (CurrentWeight + (InCount*ItemList.ItemWeight) <= MaxWeight)
	{
		//DebugText = FString::Printf(TEXT("Check if Weight %f  %f / %f "),CurrentWeight,ItemList.ItemWeight,MaxWeight);
		//Debug::Print(DebugText,FColor::Blue);
		
		//Debug::Print("IS In Full Charge");
		//Debug::Print("Is Bullet",FColor::Blue);
		const int32 BulletIndex = ItemList.ItemID - 150;
		const EBulletType BulletType = static_cast<EBulletType>(static_cast<uint8>(BulletIndex));
						
		// Add to the existing value : 기존 값에 더함

		//DebugText = FString::Printf(TEXT("Bullet Count: Type=%d, Count=%d"), static_cast<int32>(BulletType), CurrentBulletCount[BulletType]);
		//Debug::Print(DebugText,FColor::Blue);
						
		CurrentBulletCount[BulletType] += InCount; 
						
		// Debug : 디버그
		//DebugText = FString::Printf(TEXT("Updated Bullet Count: Type=%d, Count=%d , UpdateCount=%d ,CurrentWeight %f"), static_cast<int32>(BulletType),InCount, CurrentBulletCount[BulletType],CurrentWeight);
		//Debug::Print(DebugText,FColor::Blue);
						
		CurrentWeight += InCount*ItemList.ItemWeight;
						
		//DebugText = FString::Printf(TEXT("Updated Weight: ItemWeight %f = Update Weight %f / %f "),InCount*ItemList.ItemWeight,CurrentWeight,MaxWeight);
		//Debug::Print(DebugText,FColor::Blue);
						
		OutCount = 0;
	}
	// 므게 가 초과 되었을때 
	else
	{
		int32 MaxAddableCount = (MaxWeight - CurrentWeight) / ItemList.ItemWeight; 
		Debug::Print("Is Bullet",FColor::Blue);
						
		const int32 BulletIndex = ItemList.ItemID - 150;
		const EBulletType BulletType = static_cast<EBulletType>(static_cast<uint8>(BulletIndex));
						
		//DebugText = FString::Printf(TEXT("The item was found in the Itemlist. BulletType=%d"),static_cast<int32>(BulletType));
		//Debug::Print(DebugText,FColor::Blue);
						
						
						
		//DebugText = FString::Printf(TEXT("Bullet Count: Type=%d, Count=%d"), static_cast<int32>(BulletType), CurrentBulletCount[BulletType]);
		//Debug::Print(DebugText,FColor::Blue);
		// Add to the existing value : 기존 값에 더함
		CurrentBulletCount[BulletType] += MaxAddableCount; 
						
		RemainingCount -= MaxAddableCount;
						
		OutCount = RemainingCount;
		//DebugText = FString::Printf(TEXT("Updated Bullet Count: Type=%d, Count=%d , UpdateCount=%d, RemainCount=%d,CurrentWeight %f"),static_cast<int32>(BulletType),MaxAddableCount, CurrentBulletCount[BulletType],OutCount,CurrentWeight);
		//Debug::Print(DebugText,FColor::Blue);
						
		CurrentWeight += MaxAddableCount * ItemList.ItemWeight;
						
		DebugText = FString::Printf(TEXT("Updated Weight:, ItemWeight %f = Update Weight %f / %f "),MaxAddableCount*ItemList.ItemWeight,CurrentWeight,MaxWeight);
		Debug::Print(DebugText,FColor::Blue);
	}
}

// Inherited function : 상속 받은 함수 

// Item acquisition function: Adds an item to the inventory : 아이템 획득 함수: 인벤토리에 아이템을 추가하는 기능
bool UPlayerInventoryComponent::AcquireItem(int32 InItemID, int32 InCount, int32& OutRemainingCount )
{
	// Log an error and handle the exception if the DataTable is not set 
	// 데이터 테이블이 설정되지 않은 경우 로그출력 예외 처리
	if (!ItemDataTable)
	{
		//Debug::Print("ItemDataTable not found, returning False. (Critical Error)!",FColor::Red);
		//Debug::Print("Error called -> PlayerInventoryComponent.cpp(13)",FColor::Red);
		return false;
	}
	
	// Search for the corresponding item ID in the DataTable and retrieve the data :
	// 데이터 테이블에서 해당 아이템 ID를 검색하여 데이터 가져오기
	const FInventoryDataStruct* CheckDataTable = ItemDataTable->FindRow<FInventoryDataStruct>(FName(*FString::FromInt(InItemID)), TEXT(""));
	if (CheckDataTable == nullptr)
	{
		// Debug : 디버그
		//DebugText = FString::Printf(TEXT("Item ID %d not found in the data table!"), InItemID);
		//Debug::Print(DebugText,FColor::Red);
		//Debug::Print("Error called -> PlayerInventoryComponent.cpp(81)",FColor::Red);
		return false;
	}
	if (CheckDataTable->ItembIsStorable)
	{
		int32 RemainingCount = InCount;
	
		for (FInventoryItems& ItemList: InventoryItems)
		{
			if (ItemList.ItemID == InItemID )
			{
				if (ItemList.ItemType == EItemType::Ammobox)
				{
					AcquireDropAmmo(ItemList, InCount, RemainingCount,OutRemainingCount);
				}
				else
				{
					AcquireDropItem(ItemList, InCount, RemainingCount,OutRemainingCount);
				}
				return true;
			}
		}
		
	}
	return false;
}

// 2025-01-17 수정

// Item discard function: Allows dropping a specific item : 아이템 버리기 함수: 특정 아이템을 버리는 기능
void UPlayerInventoryComponent::DiscardItem(int32 InItemID, int32 InCount,int32& OutCount)
{
	int32 RemainCount =0;
	int32 DisCardCount =0;
	OutCount = InCount;
	for (FInventoryItems& ItemList: InventoryItems)
	{
		if (InItemID == ItemList.ItemID)
		{
			if (ItemList.ItemType == EItemType::Ammobox)
			{
				const int32 BulletIndex = ItemList.ItemID - 150;
				const EBulletType BulletType = static_cast<EBulletType>(static_cast<uint8>(BulletIndex));
				if (InCount <= CurrentBulletCount[BulletType])
				{
					CurrentBulletCount[BulletType] -= InCount;
					CurrentWeight -= (ItemList.ItemWeight * InCount);
					OutCount = 0;
				}
				else
				{
					RemainCount = InCount - CurrentBulletCount[BulletType];
					DisCardCount = InCount - RemainCount;
				
					CurrentWeight-= (ItemList.ItemWeight * DisCardCount);
					CurrentBulletCount[BulletType] =0;
					
					OutCount = RemainCount;
				}
			}
			else
			{
				// When the quantity is sufficient : 보유량이 충분할 때
				if (InCount <= ItemList.ItemCount)
				{
					ItemList.ItemCount -= InCount;
					CurrentWeight -= (ItemList.ItemWeight * InCount);
					OutCount = 0;
				}
				// When discarding a quantity lower than the possessed amount : 보유 수량보다 많은 수를 버릴 때
				else
				{
					RemainCount = InCount - ItemList.ItemCount;
					DisCardCount = InCount - RemainCount;
					
					CurrentWeight-= (ItemList.ItemWeight * DisCardCount);
					ItemList.ItemCount =0;
					OutCount = RemainCount;
				}
			}
			
		}
		
	}
}

// Item usage function: Enables the use of a specific item : 아이템 사용 함수: 특정 아이템을 사용하는 기능
bool UPlayerInventoryComponent::UseItem(int32 InItemID)
{
	for (FInventoryItems& ItemList: InventoryItems)
	{
		if (InItemID == ItemList.ItemID)
		{
			ItemList.ItemCount --;
			CurrentWeight -= ItemList.ItemWeight;
			
			// DebugText = FString::Printf(TEXT("Item %d Use"),InItemID);
			// Debug::Print(DebugText,FColor::Blue);
			//
			// DebugText = FString::Printf(TEXT("Weight - %f / UpdateCurrentWeight %f."),ItemList.ItemWeight,CurrentWeight);
			// Debug::Print(DebugText,FColor::Blue);
		}
	}
	return false;
}

// Item possession check function: Verifies if a specific item is held in sufficient quantity :
// 아이템 보유 여부 확인 함수: 특정 아이템이 충분한 수량으로 보유 중인지 확인
bool UPlayerInventoryComponent::HasItem(int32 InItemID, int32 InCount)
{
	Debug::Print("Debug HasItem",FColor::Red);
	for (const FInventoryItems& ItemList : InventoryItems)
	{
		if (ItemList.ItemID == InItemID && ItemList.ItemCount >= InCount)
		{
			
			return true;
		}
	}
	// Returns false if the item is not possessed : 보유하지 않은 경우 false 반환
	return false;
}

// Reload : 재장전
bool UPlayerInventoryComponent::ReloadBullet(EBulletType InBulletType, int32 InCount, int32& OutCount)
{
	// Requested number of bullets : 요청한 총알 수
	int32 RequestBullet = InCount;

	if (!CurrentBulletCount.Contains(InBulletType))
	{
		Debug::Print("No entry in CurrentBulletCount for bullet type.",FColor::Red);
		return false;
	}
	
	for (FInventoryItems& ItemList: InventoryItems)
	{
		const int32 BulletIndex = ItemList.ItemID - 150;
		const EBulletType BulletType = static_cast<EBulletType>(static_cast<uint8>(BulletIndex));
		if (BulletType == InBulletType)
		{
			if (CurrentBulletCount[InBulletType] >= RequestBullet)
			{
				CurrentBulletCount[InBulletType] -= RequestBullet;
				OutCount = RequestBullet;
				CurrentWeight -= RequestBullet * ItemList.ItemWeight;
				
				// DebugText = FString::Printf(TEXT("Reload successful: %d bullets used. Remaining: %d"),RequestBullet, CurrentBulletCount[InBulletType]);
				// Debug::Print(DebugText,FColor::Blue);
				return true;
			}
			else
			{
				if (CurrentBulletCount[InBulletType] <= 0)
				{
					Debug::Print("No bullet",FColor::Red);
					return false;
				}
				OutCount = CurrentBulletCount[InBulletType];
				CurrentWeight -= CurrentBulletCount[InBulletType]*ItemList.ItemWeight;
				CurrentBulletCount[InBulletType] =0;

				
				// DebugText = FString::Printf(TEXT("Not enough bullets. Available: %d, Requested: %d"),CurrentBulletCount[InBulletType], RequestBullet);
				// Debug::Print(DebugText,FColor::Blue);
				return false;
			}
		}
	}
	return false;
}
void UPlayerInventoryComponent::HasBullet(EBulletType InBulletType, int32& OutCount)
{
	// Pass the quantity of the specified bullet to the external caller : 해당 총알 보유량을 외부로 전달
	OutCount = CurrentBulletCount[InBulletType];
}

// 
void UPlayerInventoryComponent::DebugInventoryPrint()
{
	InventoryUI.Empty();
	// Super::DebugInventoryPrint();
	for (FInventoryItems& ItemList: InventoryItems)
	{
		// 인벤 전체 순회
		if (ItemList.ItemCount > 0)
		{
			// 있을때
			const FInventoryDataStruct* CheckDataTable = ItemDataTable->FindRow<FInventoryDataStruct>(FName(*FString::FromInt(ItemList.ItemID)), TEXT(""));
			if (CheckDataTable->ItemMaxStackCount >= ItemList.ItemCount)
			{
				AddUISlot(ItemList.ItemID,ItemList.ItemCount);
				// 슬롯 1개
			}
			else
			{
				int32 RemainCount = ItemList.ItemCount;
				while (RemainCount > 0 )
				{
					if (RemainCount <= CheckDataTable->ItemMaxStackCount)
					{
						AddUISlot(ItemList.ItemID,RemainCount);
						break;
					}
					else
					{
						AddUISlot(ItemList.ItemID,CheckDataTable->ItemMaxStackCount);
						RemainCount -=  CheckDataTable->ItemMaxStackCount;
					}
				}
			}
		}
	}
	for (const TPair<EBulletType, int32>& Pair : CurrentBulletCount)
	{
		if (Pair.Value <= 0)
		{
			// DebugText = FString::Printf(TEXT("No Bullet %d , Count %d "),Pair.Key,Pair.Value );
			// Debug::Print(DebugText,FColor::Blue);
		}
		else
		{
			const int32 BulletIndex = static_cast<int32>(Pair.Key) + 150;
			AddUISlot( BulletIndex, Pair.Value);
		}
	}
	// InventoryUI 배열이 비었는지 확인
	if (InventoryUI.Num() == 0)
	{
		DebugText = TEXT("InventoryUI 배열이 비어 있습니다.");
		Debug::Print(DebugText, FColor::Red);
		return;
	}

	InventoryUI.Sort([](const FInventoryUI& A, const FInventoryUI& B)
   {
	   return A.ItemID < B.ItemID;
   });
	

	// 정렬 결과를 확인하기 위한 디버그 출력
	for (FInventoryUI& Item : InventoryUI)
	{
		DebugText = FString::Printf(TEXT("ItemID: %d, ItemName: %s , ItemWeight %f , ItemCount %d"), Item.ItemID, *Item.ItemDisplayName, Item.ItemWeight, Item.ItemCount);
		Debug::Print(DebugText,FColor::Blue);
	}
	DebugText = FString::Printf(TEXT("Current Weight %f , MaxWeight %f "),CurrentWeight,MaxWeight );
	Debug::Print(DebugText,FColor::Blue);
}

TArray<FInventoryUI> UPlayerInventoryComponent::GetInventoryData()
{
	DebugInventoryPrint();
	return InventoryUI;
}

