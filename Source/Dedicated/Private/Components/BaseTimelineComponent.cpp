// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BaseTimelineComponent.h"

#include "BaseDebugHelper.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UBaseTimelineComponent::UBaseTimelineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	mParachuteTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ParachuteTimeline"));
	mCameraLocationWithArmLengthTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("CameraLocationTimeline"));
}


void UBaseTimelineComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!mParachuteScaleCurve)
	{
		mParachuteScaleCurve = NewObject<UCurveFloat>(this, TEXT("GeneratedScaleCurve"));
		if (mParachuteScaleCurve)
		{
			mParachuteScaleCurve->FloatCurve.AddKey(0.0f, 0.0f);
			mParachuteScaleCurve->FloatCurve.AddKey(1.0f, 1.0f);
		}
	}

	if (!mCameraLocationWithArmLengthCurve)
	{
		mCameraLocationWithArmLengthCurve = NewObject<UCurveFloat>(this, TEXT("GeneratedLocationWithArmLengthCurve"));
		if (mCameraLocationWithArmLengthCurve)
		{
			mCameraLocationWithArmLengthCurve->FloatCurve.AddKey(0.0f, 0.0f);
			mCameraLocationWithArmLengthCurve->FloatCurve.AddKey(0.3f, 1.0f);
		}
	}
}

void UBaseTimelineComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (mCameraLocationWithArmLengthTimeline)
		mCameraLocationWithArmLengthTimeline->TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UBaseTimelineComponent::UpdateParachuteScale(const float _Value)
{
	FVector NewScale = FVector(_Value*1.f);
	mParachute->SetActorScale3D(NewScale);

}

void UBaseTimelineComponent::StartParachuteScale(AActor* _Parachute)
{
	if (!_Parachute)
	{
		Debug::Print("Invalid Parachute",FColor::Red);
		return;
	}
	mParachute = _Parachute;

	
	FOnTimelineFloat TimeLineCallback;
	TimeLineCallback.BindUFunction(this,FName("UpdateParachuteScale"));
	mParachuteTimeline->AddInterpFloat(mParachuteScaleCurve,TimeLineCallback,FName("UpdateParachuteScale"));
	mParachuteTimeline->PlayFromStart();

}

void UBaseTimelineComponent::StartCameraLocation(const FVector_NetQuantize _Camera_Location,
	const float _CameraArmLength, USpringArmComponent* _CameraSpringArm)
{
	if (!_CameraSpringArm)
	{
		Debug::Print("BaseTimeLine :: Invalid CameraLocation",FColor::Red);
		return;
	}
	if (!mCameraLocationWithArmLengthTimeline)
	{
		Debug::Print("BaseTimeLine :: TimeLine is Null",FColor::Red);
		return;
	}

	
	mCameraLocation = _Camera_Location;
	mCameraArmLength = _CameraArmLength;
	mCameraSpringArm = _CameraSpringArm;

	mInitCameraLocation = mCameraSpringArm->GetRelativeLocation();
	mInitCameraArmLength = mCameraSpringArm->TargetArmLength;

	FOnTimelineFloat TimeLineCallback;
	TimeLineCallback.BindUFunction(this,FName("UpdateCameraLocationAndLength"));
	mCameraLocationWithArmLengthTimeline->AddInterpFloat(mCameraLocationWithArmLengthCurve,TimeLineCallback,FName("UpdateCameraLocationAndLength"));
	
	
	mCameraLocationWithArmLengthTimeline->PlayFromStart();
}

void UBaseTimelineComponent::UpdateCameraLocationAndLength(const float _Value)
{
	if (!mCameraSpringArm)
	{
		Debug::Print("Invalid CameraSpringArm",FColor::Red);
		return;
	}
	
	float NewCameraArmLength = UKismetMathLibrary::Lerp(mInitCameraArmLength,mCameraArmLength,_Value);
	FVector NewCameraLocation = UKismetMathLibrary::VLerp(mInitCameraLocation,mCameraLocation,_Value);
	
	mCameraSpringArm->TargetArmLength = NewCameraArmLength;
	mCameraSpringArm->SetRelativeLocation(NewCameraLocation);

}
