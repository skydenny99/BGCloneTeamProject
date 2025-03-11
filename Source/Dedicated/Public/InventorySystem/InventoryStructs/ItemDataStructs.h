#pragma once
//
//#include "CoreMinimal.h"
//#include "Engine/DataTable.h"
//#include "ItemDataStructs.generated.h"
//
//// 아이템 타입을 분류하기 위한 열거형
//UENUM(BlueprintType)
//enum class EItemType : uint8
//{
//	// 주무기 (메인 화기)
//	mainweapon UMETA(DisplayName = "MainWeapon"),
//	// 보조무기 (서브 화기)
//	subweapon UMETA(DisplayName = "SubWeapon"),
//	// 근접 무기
//	melee UMETA(DisplayName = "Melee"),
//	// 탄약 상자
//	ammobox UMETA(DisplayName = "AmmoBox"),
//	// 치유 아이템 (예: 구급상자, 붕대)
//	healing UMETA(DisplayName = "Healing"),
//	// 투척 아이템 (예: 수류탄)
//	throwing UMETA(DisplayName = "Throwing"),
//	// 방어구 조끼 (피해 감소)
//	vest UMETA(DisplayName = "Vest"),
//	// 가방 (인벤토리 용량 증가)
//	backpack UMETA(DisplayName = "BackPack"),
//	// 헬멧 (머리 보호)
//	helmet UMETA(DisplayName = "Helmet"),
//	// 부착물 (예: 조준경, 소음기)
//	attachments UMETA(DisplayName = "Attachments")
//};
//
///** 아이템을 검색하기 위한 ID 데이터 */
//USTRUCT(BlueprintType)
//struct FItemIdData
//{
//	GENERATED_BODY();
//
//	/** 아이템 고유 ID */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemIdData")
//	int32 ItemID; // 예: 1은 AKM, 251은 수류탄
//
//	/** 검색용 아이템 이름 */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemIdData")
//	FString ItemName; // 예: "akm" (내부 시스템에서 사용되는 식별 이름)
//};
//
///** UI 표시용 데이터 */
//USTRUCT(BlueprintType)
//struct FItemUiData
//{
//	GENERATED_BODY();
//
//	/** UI 표시용 아이템 이름 */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemUiData")
//	FString DisplayName; // 예: "AKM" (유저에게 표시되는 이름)
//
//	/** 아이템 설명 */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemUiData")
//	FString Description; // 예: "표준형 돌격 소총으로, 화력이 보통 수준입니다."
//};
//
///** 아이템의 수치 관련 데이터 */
//USTRUCT(BlueprintType)
//struct FItemNumericData
//{
//	GENERATED_BODY();
//
//	/** 인벤토리에서 아이템의 무게 */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemNumericData")
//	float Weight; // 예: 구급상자 10, 12게이지 총알 1.25
//
//	/** 해당 아이템의 스택 가능 여부 */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemNumericData")
//	bool bIsStackable; // 예: 무기 스택 False, 회복템 스택 True
//
//	/** 해당 아이템의 최대 스택 수 */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemNumericData")
//	int32 MaxStackCount; // 예: 구급상자 스택수 무한, 12게이지 총알 60발 제한
//
//	/** 해당 아이템의 가방에 보관 가능 여부 */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemNumericData")
//	bool bIsStorable; // 예: 무기 보관 False, 회복템 보관 True
//};
//
///** 아이템 에셋 패스 */
//USTRUCT(BlueprintType)
//struct FItemAssetData
//{
//	GENERATED_BODY();
//
//	/** UI에 사용되는 아이콘 이미지 경로 */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemAssetData")
//	FString IconPath; // 예: "/Game/UI/Icons/AKM.png"
//};
//
///** RowBase 데이터 테이블 상속받아 FItemData 생성 */
//USTRUCT(BlueprintType)
//struct FItemData : public FTableRowBase
//{
//	GENERATED_BODY();
//
//	/** 아이템 ID 데이터 */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
//	FItemIdData ItemIdData;
//
//	/** 아이템 타입 (EItemType 열거형을 사용) */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
//	EItemType ItemType; // 아이템 타입 추가
//
//	/** UI 표시용 데이터 */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
//	FItemUiData ItemUiData;
//
//	/** 아이템의 수치 데이터 */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
//	FItemNumericData ItemNumericData;
//
//	/** 아이템 에셋 데이터 */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
//	FItemAssetData ItemAssetData;
//};