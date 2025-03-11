// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDebugHelper.h"
#include "Characters/PubgBaseCharacter.h"
#include "BaseType/BaseEnumType.h"
#include "BaseType/BaseStructType.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PubgCharacter.generated.h"


struct FInputActionInstance;
class ABasePlane;
class APubgInventoryCharacter;
class ABaseFieldItem;
class AHUDBase;
class USpringArmComponent;
class UCameraComponent;
class UPubgAnimInstance;



/**
 * 
 */
UCLASS()
class DEDICATED_API APubgCharacter : public APubgBaseCharacter
{
	GENERATED_BODY()

public:
	APubgCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


protected:
	UFUNCTION()
	void Input_Look_Custom(const FInputActionValue& InputValue);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mouse Settings")
	float MouseSensitivityX = 1.0f;  // 좌우 감도 (기본값 1.0f)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mouse Settings")
	float MouseSensitivityY = 1.0f; // 상하 감도

	UFUNCTION()
	void SetMouseSensitivity(float NewSensitivityX, float NewSensitivityY);

	
public:
	UPROPERTY(ReplicatedUsing=OnRep_UpdateControllerRotationYaw)
	bool bUpdateControllerRotationYaw;
	
protected:
	
#pragma region Status
	/** 캐릭터가 BodyMode를 바꿀때. 적용할 이동속도 값. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status");
	float StandSpeed = 350.f;

	/** 캐릭터가 BodyMode를 바꿀때. 적용할 이동속도 값. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status");
	float CrouchSpeed = 300.f;

	/** 캐릭터가 BodyMode를 바꿀때. 적용할 이동속도 값. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status");
	float ProneSpeed = 100.f;

	/** 플레이어가 줌을 안할때의 카메라 위치값. */
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Status");
	FVector Init_Location_CameraSpringArm = FVector(0.f,25.f,88.f);

	/** 플레이어가 줌을 할 경우 카메라 위치값 */
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Status");
	FVector Init_Location_ZoomCamera = FVector(0.f,50.f,60.f);

	/** 캐릭터가 F,FL,FR 말고 다른 방향으로 이동할려고 할 경우 이동속도 제한할 때 쓸 변수 */
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Status");
	float PlayerMoveDirection;

	/** 줌을 할 경우 카메라 거리 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status");
	float ZoomTargetArmLength = 50.f;

	/** 기본 카메라 거리 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status");
	float mTargetArmLength = 200.f;

	/** 인벤토리 여부 */
	UPROPERTY(VisibleAnywhere, Replicated ,BlueprintReadOnly, Category = "Status");
	bool bIsInventoryOpen;

	/** 자유낙하 */
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_FreeFalling ,BlueprintReadOnly, Category = "Status");
	bool bIsFreeFalling = true;
	
	/** 자유낙하 관련 변수 */
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_FreeFalling ,BlueprintReadOnly, Category = "Status");
	bool bIsFreeFallingLock = false;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Status");
	bool bHasPlaneExit;

	
	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status");
	TSubclassOf<AActor> ParachuteClass;

	/** 낙하산을 스폰하면 캐싱할 변수 */
	UPROPERTY( VisibleDefaultsOnly, BlueprintReadOnly, Category = "Status");
	AActor*  Parachute;
public:
	// 기울이기 중인지 판별 변수
	/**  */
	UPROPERTY(ReplicatedUsing=OnRep_IsLeaning , BlueprintReadWrite, Category = "Status");
	float mIsLeaning;

	// Leaning을 할 때. 썻지만. 필요한지는 테스트는 아직 안해봄 사용하긴 하지만 필요없을 경우 삭제 요망.
	UFUNCTION()
	void OnRep_IsLeaning();

	UFUNCTION()
	void OnRep_FreeFalling();
	
	UFUNCTION(NetMulticast, Reliable, Category = "Status")
	void Multicast_SetLeaning(const float _NewValue);

	UFUNCTION(Server, Reliable, Category = "Status")
	void Server_SetLeaning(const float _NewValue);
	
	UFUNCTION(Client, Reliable)
	void Client_PlaneInputContext(const bool bIsPlane);
	
protected:

	// 현재 Body모드에 맞는 카메라 위치값 밷는 함수
	/**  */
	FVector GetPoseCameraLocation();
	
	// 2025/01/20 JW FallingSystem

	//FallDamageHeight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float FallHeight = -1500.f;
	
	//Falling Damage
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Status");
	float FallDamage =0.f;
	
	UPROPERTY(EditAnywhere, Category = "Status");
	TSubclassOf<UGameplayEffect> FallDamageEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Inventory");
	TObjectPtr<APubgInventoryCharacter> InventoryCharacter;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Inventory");
	TSubclassOf<APubgInventoryCharacter> InventoryCharacterClass;

	
#pragma endregion
	
#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FPP_Camera;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	// ACineCameraActor* FPP_CineCamera;

#pragma endregion

#pragma region Input
private:
	
	
	/** 플레이어 Body값을 담고 있는 변수, AnimInstance와 동기화 필수 이며, Anim은 관련 변수로 애니메이션을 실행함 */
	UPROPERTY(ReplicatedUsing=OnRep_PlayerBodyMode, EditAnywhere, BlueprintReadWrite, Category = "PlayerActionMode", meta = (AllowPrivateAccess = "true"))
	EPlayerActionMode PlayerBodyMode = EPlayerActionMode::Stand;

	/** 플레이어 Equip값을 담고 있는 변수, AnimInstance와 동기화 필수 이며, Anim은 관련 변수로 애니메이션을 실행함 */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "PlayerActionMode", meta = (AllowPrivateAccess = "true"))
	EPlayerActionMode PlayerEquipMode = EPlayerActionMode::Unarmed;

	//플레이어가 가만히 점프 할 경우와 가만히 점프할 경우를 판별하기 위해 생성된 변수, ( 현재는 네트워크 적용하면서 사용 안함, 언젠간 해야함)
	UPROPERTY(Replicated)
	bool bHasJump;

	// 플레이어가 Leaning을 하는중인지 판별 변수
	UPROPERTY(Replicated)
	bool bIsLanding;

	// 플레이어가 Zoomin을 하고 있는지 판별 변수
	UPROPERTY(Replicated)
	bool bIsZoomIn;
	
	// 플레이어가 Turn in Place에 설정된 각도 이전에 움직일 경우 해당 방향으로 회전하기 위한 변수 ( 버그가 많아 현재는 사용 X )
	UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadWrite, Category = "Turn", meta = (AllowPrivateAccess = "true"))
	bool bHasTurn;

	//UI용 인벤토리 온오프 함수, 현재 온 상태인지는 ABaseHud에 존재.
	UFUNCTION()
	void ToggleInventory();

	// PickupMongtage를 실행할 변수, Setter 별도 존재.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* PickupMontage;

	// 각 포즈별 PickUp몽타주을 담고 있는 배열, 총 36개정도?
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	TMap<EPlayerActionMode, FPickupMontageCollection> PickupMontageMap;

	
	//UAnimMontage* GetPickupMontage();

	//PickupMontage 변수에 현 상황에 맞는 PickupMontage 대입, Getter로 안한 이유는 RPC를 하기위해. ( RPC는 Return 형태는 못함 )
	UFUNCTION(NetMulticast, Reliable, Category = "Pickup")
	void SetupPickupMontage();

	// 현재 몽타주에 노티파이 트리거 되면 bIsPick이 False가 되는데 모종의 사유로 호출이 안될경우. 타이머로 일정 시간 후 해제 하도록.
	UPROPERTY()
	FTimerHandle PickUpTimerHandle;

	// 지정한 시간 뒤 Pickup Lock 해제
	void PickupCompleted();
	
	/*UPROPERTY(Replicated,VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AActor* BeforeActor = nullptr;*/

	// 플레이어가 바라보고 있는 아이템 정보 저장 변수 ( 가까이 X, 바라보고있는 O)
	UPROPERTY(Replicated,VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FNearActor NearActor;

	// Pickup Lock 변수
	bool bIsPickUp = false;
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION(Server, Reliable)
	void Server_SetZoom(bool InZoomIn);

	UFUNCTION(Server, Reliable)
	void Server_ActiveControllerYaw();

	// 옷
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Costume")
	USkeletalMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Costume")
	USkeletalMeshComponent* LegMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Costume")
	USkeletalMeshComponent* FeetMesh;


	
	
	
public:
	//플레이어 마우스 우클릭에 Binding된 변수 ( ZoomIn )
	UFUNCTION()
	void FlipZoomIn();
	UFUNCTION()
	void FlipZoomIn_Release();

	
	
	// 1인칭 줌 관련
	bool bIsButtonPressed;
	float ShortPressThreshold;

	UFUNCTION()
	void OnZoomButtonPressed(const FInputActionInstance& ActionInstance);
	
	UFUNCTION()
	void OnZoomButtonReleased(const FInputActionInstance& ActionInstance);

	UPROPERTY()
	bool bFlipFlopState = false;

	// 줌 함수
	UFUNCTION(BlueprintCallable)
	void FPP_Zoom();
	


public:
	//문
	UPROPERTY(EditAnywhere)
	float InteractLineTraceLength = 350.f;

	UFUNCTION()
	void Input_MoveCheck(const FInputActionValue& InputValue);
	UFUNCTION()
	void Input_LookCheck(const FInputActionValue& InputValue);
	
	
	// Legacy (플레이어가 Turn In Place 하기전 움직을 때. 판별하는 함수, 현재는 버그가 많아 사용 X )
	UFUNCTION(Server, Reliable)
	void CheckNotTurn();
	
	//플레이어 모드와 AnimInstance의 Enum Change
	UFUNCTION(Server, Reliable)
	void Input_Crouch();
	
	//플레이어 모드와 AnimInstance의 Enum Change
	UFUNCTION(Server, Reliable)
	void Input_Rifle();

	// 현재 이동속도 / 2
	UFUNCTION(Server, Reliable)
	void Input_LCtrl();
	UFUNCTION(Server, Reliable)
	void Input_LCtrl_Release();

	// 현재 이동속도 * 2
	UFUNCTION(Server, Reliable)
	void Input_LShift();
	UFUNCTION(Server, Reliable)
	void Input_LShift_Release();

	UFUNCTION(Server, Reliable)
	void Input_Prone();

	// 그냥 CharacterJump를 할 순 있지만. 플레이어가 움직이는 도중 점프를 판별하기 위해 생성
	UFUNCTION(NetMulticast, Reliable)
	void PubgJump();
	UFUNCTION(NetMulticast, Reliable)
	void PubgStopJump();
	
	//Mouse 움직임에 바인딩된 함수, 특정 각도에 도달 시 Turn 함수 호출.
	UFUNCTION(Server, Reliable)
	void CheckCharacterAngle();

	// 타겟 Item Overlay 활성화 로직
	//UFUNCTION(Client, Reliable)
	void CheckCloseActor();

	// 현재 Overlap Actor 중 Item이 없거나. Overlap된게 아무것도 없을 경우에만 실행.
	UFUNCTION(Client, Reliable)
	void InitCloseActor();

	// 현재 Overlap Actor중 BaseInteractComponent가 있는지 존재 유무
	bool CheckItemActorOnOverlapActors();

	//Turn Custom Event
	UFUNCTION(BlueprintImplementableEvent, Category = "Turn")
	void PubgTurn();

	//Server Turn
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Turn")
	void Server_OnPubgTurn();

	UFUNCTION(NetMulticast, Reliable, Category = "Turn")
	void Multicast_OnPubgTurn();

	//Change CameraArmLocation
	UFUNCTION(BlueprintImplementableEvent, Category = "Camera")
	void Change_Camera_Location(const FVector_NetQuantize _Camera_Location, const float CameraArmLength);

	void OnChangeCameraLocation(const FVector& _Camera_Location, const float _TargetArmLenth);
	
	// Get TargetArmLength, if Zoomin
	float GetTargetArmLength();

	// Montage Play Detected
	virtual bool IsMontagePlaying() override;

	// Stop Montage
	virtual void SkipCurrentMontage() override;

	//Leaning
	void Input_Leaning(const FInputActionValue& InputValue);


	//플레이어 F키 = Interact 키에 바인딩 된 함수.
	void Input_Interacting();
	
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_FindDoor();
	
	UFUNCTION(server, Reliable)
	void Server_DoorInput_Interacting();

	ABaseFieldItem* GetNearestFieldItem();

	UPROPERTY()
	TObjectPtr<AHUDBase> PlayerHUDBase;

	// ?
	UFUNCTION(NetMulticast, Reliable)
	void PlayPickUpMontage(EPickMode PickMode);

	void Input_ExitPlane();
	
	//비행기 점프
	UFUNCTION(Server, Reliable, WithValidation)
	void PlaneExitPlayer();
	bool PlaneExitPlayer_Validate();
	void PlaneExitPlayer_Implementation();
	
	UFUNCTION(Server, Reliable)
	void AttachPlane();
	void AttachPlane_Implementation();
	
	/** 비행기에 어태치되었는지 여부 (서버-클라이언트 동기화) */
	UPROPERTY(Replicated)
	bool bIsAttachedToPlane = false;

	/** 현재 어태치된 소켓의 위치 (서버-클라이언트 동기화) */
	UPROPERTY(Replicated)
	FTransform AttachedSocketTransform;
	
#pragma endregion


	UPROPERTY(Replicated)
	bool FPP_MODE = false;
	UFUNCTION(BlueprintCallable)
	bool GetFPP_MODE() const { return FPP_MODE; }
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetFPP_MODE(bool bVal);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetFPP_MODE(bool bVal);

	UPROPERTY()
	bool FPP_MODE_Client = false;
	FORCEINLINE bool GetFPP_MODE_Client() const { return FPP_MODE_Client; }
	UFUNCTION(BlueprintCallable)
	void SetFPP_MODE_Client(bool bVal){FPP_MODE_Client = bVal; }

	UPROPERTY(ReplicatedUsing=OnRep_MaxWalkSpeed)
	float CurrentMaxWalkSpeed;
	
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = "true"))
	UPubgAnimInstance* PubgAnimInstance;
	
	
public:
	//플레이어 행동 변경 함수
	UFUNCTION(NetMulticast, Reliable,BlueprintCallable, Category = "PlayerState")
	void ChangePlayerBodyMode(const EPlayerActionMode NewPlayerBodyMode);

	//플레이어 행동 변경 함수
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "PlayerState")
	void ChangePlayerEquipMode(const EPlayerActionMode NewPlayerEquipMode);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "PlayerState")
	void ChangePlayerGunType(const EGunName& NewGunType);
	
	// 전달 받은 행동에 따라. 카메라 앵글 Pitch를 제한하는 함수
	UFUNCTION(blueprintCallable, Category = "PlayerState")
	void LimitCameraAngle(const EPlayerActionMode NewPlayerBodyMode);
	
	// Actor Change Rotation for Timeline When Not Turn
	UFUNCTION(BlueprintImplementableEvent, Category = "Turn")
	void PubgNotTurn();

	// server
	UFUNCTION()
	void OnRep_PlayerBodyMode();

	// Apply Player Speed on Server
	// UFUNCTION(NetMulticast, Unreliable)
	// void OnUpdateSpeed();
	//
	// // Apply Player Speed on Server
	// UFUNCTION(NetMulticast, Unreliable)
	// void OnUpdateSpeedWithValue(const float NewSpeed);

	
	//UFUNCTION(Client, Reliable)
	void GetCloseActorToScreenCenter();

	//타겟 위치값 가져와서 FNearActor의 EPickMode 설정
	UFUNCTION(Netmulticast, Reliable)
	void Multicast_SetPickMode(const float LocationZ);

	UFUNCTION(Server, Reliable)
	void Server_SetPickMode(const float LocationZ);


	/**
	 * @brief 인벤토리용 캐릭터의 모션 변경 함수
	 * @note siwtch안에 기존 Enum에 추가하기 싫어서 안쓰는거 재활용하고있음
	 */
	UFUNCTION(Client, Reliable)
	void Client_InventoryChangeEquip();

	UFUNCTION()
	void OnRep_MaxWalkSpeed();

	UFUNCTION()
	void OnRep_UpdateControllerRotationYaw();

private:
	//점프가능 여부 Boolean
	UPROPERTY(VisibleAnywhere)
	bool CanExit = false;



public:
	virtual void PlayerDead() override;

	UFUNCTION(Client, Reliable)
	void Client_SummaryWidget(const bool bIs1st, const FString& OwnerName);

	
protected:
	//캐릭터가 F,FL,FR 말고 다른 방향으로 이동할려고 할 경우 이동속도 제한할 때 쓸 변수
	UFUNCTION()
	void GetPlayerMoveDirection();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void Landed(const FHitResult& Hit) override;

public:
	FORCEINLINE EPlayerActionMode GetPlayerBodyMode() const { return PlayerBodyMode; }
	FORCEINLINE EPlayerActionMode GetPlayerEquipMode() const { return PlayerEquipMode; }
	FORCEINLINE bool GetHasTurn() const { return bHasTurn; }
	FORCEINLINE void SetHasTurn(const bool _Bool) { bHasTurn = _Bool; }
	FORCEINLINE void SetIsLanding(const bool _Bool) { bIsLanding = _Bool; }
	FORCEINLINE bool GetZoomIn() const {return bIsZoomIn;}
	FORCEINLINE void SetPickUp(const bool _Bool) { bIsPickUp = _Bool; }
	FORCEINLINE bool GetPickUp() const {return bIsPickUp;}
	AHUDBase* GetHUDBase();
	FORCEINLINE APubgInventoryCharacter* GetPubgInventoryCharacter() const {return InventoryCharacter;}
	FORCEINLINE bool GetFreeFalling() const {return bIsFreeFalling;}

	UFUNCTION(Server, Reliable)
	void Server_FreeFalling();

	void StartFreeFalling();

	UFUNCTION(Server, Reliable)
	void Server_StartFreeFalling();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartFreeFalling();

	UFUNCTION(Client, Reliable)
	void Client_ActiveFreeFallingUI();

	UFUNCTION(Client, Reliable)
	void Client_DeActiveInteractWidget();

	UFUNCTION(Client, Reliable)
	void Client_ToggleInteractWidget(const bool _Active);

	UFUNCTION(Client, Reliable)
	void Client_CrosshairTrigger(const bool _Active);
	
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_FreeFalling();

	/** 현재 캐릭터가 땅과의 거리 측정  주로 낙하산 관련 시 호출됨*/
	float GetDistanceToGround();

	//비행기 점프
	FORCEINLINE void SetCanExit(const bool Value) { CanExit = Value; Debug::Print("CanExit %f",CanExit, FColor::Yellow); }
	
	float GetBodyModeSpeed() const;

	void UpdateInventory();




public:
	FORCEINLINE UCameraComponent* GetFPP_Camera() const { return FPP_Camera; }
};






