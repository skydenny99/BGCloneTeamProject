#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventoryDataStruct.generated.h"

class ABaseFieldItem;
// 아이템 타입을 분류하기 위한 열거형
UENUM(BlueprintType)
enum class EItemType : uint8
{
	// 주무기 (메인 화기)
	Mainweapon UMETA(DisplayName = "MainWeapon"),
	// 보조무기 (서브 화기)
	Subweapon UMETA(DisplayName = "SubWeapon"),
	// 근접 무기
	Melee UMETA(DisplayName = "Melee"),
	// 탄약 상자
	Ammobox UMETA(DisplayName = "AmmoBox"),
	// 치유 아이템 (예: 구급상자, 붕대)
	Healing UMETA(DisplayName = "Healing"),
	// 투척 아이템 (예: 수류탄)
	Throwing UMETA(DisplayName = "Throwing"),
	// 방어구 조끼 (피해 감소)
	Vest UMETA(DisplayName = "Vest"),
	// 가방 (인벤토리 용량 증가)
	Backpack UMETA(DisplayName = "BackPack"),
	// 헬멧 (머리 보호)
	Helmet UMETA(DisplayName = "Helmet"),
	// 부착물 (예: 조준경, 소음기)
	Attachments UMETA(DisplayName = "Attachments"),
	// 기본 값
	DefualtItemValue UMETA(DisplayName = "DefualtItemValue")
};

/** RowBase 데이터 테이블 상속받아 FInventoryDataStruct 생성 */
USTRUCT(BlueprintType)
struct FInventoryDataStruct : public FTableRowBase
{
	GENERATED_BODY();

	/** 아이템 고유 ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemId")
	int32 ItemID = 0; // 예: 1은 AKM, 251은 수류탄

	/** 검색용 아이템 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemId")
	FString ItemName; // 예: "akm" (내부 시스템에서 사용되는 식별 이름)

	/** 아이템 타입 (EItemType 열거형을 사용) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemType")
	EItemType ItemType = EItemType::DefualtItemValue; // 아이템 타입 추가

	/** UI 표시용 아이템 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemUi")
	FString ItemDisplayName; // 예: "AKM" (유저에게 표시되는 이름)

	/** 아이템 설명 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemUi")
	FString ItemDescription; // 예: "표준형 돌격 소총으로, 화력이 보통 수준입니다."

	/** 인벤토리에서 아이템의 무게 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemNumeric")
	float ItemWeight =0.0f; // 예: 구급상자 10, 12게이지 총알 1.25

	/** 해당 아이템의 스택 가능 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemNumeric")
	bool ItembIsStackable = false; // 예: 무기 스택 False, 회복템 스택 True

	/** 해당 아이템의 최대 스택 수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemNumeric", meta = (EditCondition = "bIsStackable"))
	int32 ItemMaxStackCount= 0; // 예: 구급상자 스택수 무한, 12게이지 총알 60발 제한

	/** 해당 아이템의 가방에 보관 가능 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemNumeric")
	bool ItembIsStorable = false; // 예: 무기 보관 False, 회복템 보관 True

	/** UI에 사용되는 아이콘 이미지 경로 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemAssetPath")
	FString ItemIconPath; // 예: "/Game/UI/Icons/AKM.png"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemAssetPath")
	UTexture2D* ItemIcon;
	
	// Field Item Actor Template Class
	UPROPERTY(EditAnywhere, Category = "ItemAssetPath")
	TSoftClassPtr<ABaseFieldItem> FieldItemClass;
	
	// Actual Item Actor Template Class
	UPROPERTY(EditAnywhere, Category = "ItemAssetPath")
	TSoftClassPtr<AActor> ItemActorClass;
	
	// Item 로드 경로 실제 장착을 위해
	UPROPERTY(EditAnywhere, Category = "ItemAssetPath")
	FString ItemLoadPath;

	UPROPERTY(EditAnywhere, Category = "DisplayName")
	FText DisplayName;
	
	UPROPERTY(EditAnywhere, Category = "Description")
	FText Description ;
};

// 인벤토리 구조체 
USTRUCT(BlueprintType)
struct FInventoryItems
{
	GENERATED_BODY()

	/** 아이템 고유 ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemId")
	int32 ItemID = 0; // 예: 1은 AKM, 251은 수류탄
	
	/** 아이템 타입 (EItemType 열거형을 사용) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemType")
	EItemType ItemType = EItemType::DefualtItemValue; // 아이템 타입 추가

	/** 인벤토리에서 아이템의 무게 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemNumeric")
	float ItemWeight =0.0f; // 예: 구급상자 10, 12게이지 총알 1.25
	
	// 아이템 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemNumeric")
	int32 ItemCount = 0;
};

// UI 전달용 구조체
// 출력과 정렬에 필요한 것들만
USTRUCT(BlueprintType)
struct FInventoryUI
{
	GENERATED_BODY()
	
	/** 아이템 고유 ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemId")
	int32 ItemID = 0; // 예: 1은 AKM, 251은 수류탄

	/** UI 표시용 아이템 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemUi")
	FString ItemDisplayName; // 예: "AKM" (유저에게 표시되는 이름)

	/** 아이템 설명 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemUi")
	FString ItemDescription; // 예: "표준형 돌격 소총으로, 화력이 보통 수준입니다."

	/** 인벤토리에서 아이템의 무게 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemNumeric")
	float ItemWeight =0.0f; // 예: 구급상자 10, 12게이지 총알 1.25

	/** UI에 사용되는 아이콘 이미지 경로 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemAssetPath")
	FString ItemIconPath; // 예: "/Game/UI/Icons/AKM.png"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemAssetPath")
	UTexture2D* ItemIcon;

	// 아이템 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemNumeric")
	int32 ItemCount = 0;
};

