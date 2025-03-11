// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h" 
#include "BaseDoor.generated.h"


class UBoxComponent;
class UStaticMeshComponent;
class UTimelineComponent;
class APubgCharacter;

UCLASS()
class DEDICATED_API ABaseDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseDoor();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; // 🔥 반드시 `override` 키워드 추가

	// 문 상호 작용 작동 구현
	//UFUNCTION(NetMulticast, Reliable)
	UFUNCTION()
	void OnInteract();
	//FUNCTION(NetMulticast, Reliable)
	UFUNCTION()
	void SetDoorOnSameSide();
	
	UPROPERTY(Replicated)
	APubgCharacter* Character;
	
private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
	FString DebugText; 
protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Component")
	USceneComponent* DefaultSceneRoot;
	
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* DoorFrame;
	
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* Door;
	
	//UPROPERTY(EditAnywhere, Category = "Component")
	//UBoxComponent* CollisionBox;
	
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UTimelineComponent* Timeline;
	
	UPROPERTY(VisibleAnywhere, Category = "OpenFunction")
	UCurveFloat* CurveFloat;

	UPROPERTY(VisibleAnywhere, Category = "OpenFunction")
	bool bIsDoorClosed;

	UPROPERTY(VisibleAnywhere, Category = "OpenFunction")
	bool bDoorOnSameSide;
	
	UPROPERTY(VisibleAnywhere, Category = "OpenFunction")
	float DoorRotateAngle = 90.f;

	
	
	UFUNCTION()
	void OpenDoor( float Value); //Bind Function

	
	
};



