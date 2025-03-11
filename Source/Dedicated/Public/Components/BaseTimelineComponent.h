// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseTimelineComponent.generated.h"


class USpringArmComponent;
class UTimelineComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEDICATED_API UBaseTimelineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBaseTimelineComponent();
	
	UFUNCTION()
	void UpdateParachuteScale(const float _Value);

	UFUNCTION()
	void StartParachuteScale(AActor* _Parachute);

	UFUNCTION()
	void StartCameraLocation(const FVector_NetQuantize _Camera_Location, const float _CameraArmLength, USpringArmComponent* _CameraSpringArm);
	UFUNCTION()
	void UpdateCameraLocationAndLength(const float _Value);
	

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	


private:
	UPROPERTY()
	UTimelineComponent* mParachuteTimeline;
	UPROPERTY()
	UCurveFloat* mParachuteScaleCurve;
	UPROPERTY()
	AActor* mParachute;


	UPROPERTY()
	UTimelineComponent* mCameraLocationWithArmLengthTimeline;
	UPROPERTY()
	UCurveFloat* mCameraLocationWithArmLengthCurve;
	UPROPERTY()
	FVector_NetQuantize mCameraLocation;
	UPROPERTY()
	float mCameraArmLength;
	UPROPERTY()
	USpringArmComponent* mCameraSpringArm;
	UPROPERTY()
	float mInitCameraArmLength;
	UPROPERTY()
	FVector_NetQuantize mInitCameraLocation;
	
};
