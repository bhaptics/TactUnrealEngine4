// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "HapticStructures.h"
#include "FeedbackFile.h"
#include "HapticManagerComponent.generated.h"


UCLASS(ClassGroup = (bHaptics), meta = (BlueprintSpawnableComponent))
class HAPTICSMANAGER_API UHapticManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHapticManagerComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Submit a haptic feedback file to be played by the Player.
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit Feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void SubmitFeedback(UFeedbackFile* Feedback);

	//Submit a haptic feedback file and transform it by a given RotationOption.
	//This call will only rotate vest feedback files, with other devices being kept the same.
	//Provide an AltKey to uniquely identify this feedback.
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit Feedback with Transform",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void SubmitFeedbackWithTransform(UFeedbackFile* Feedback, const FString &AltKey, FRotationOption Option);

	//Submit a haptic feedback file, and scale the intensity and duration by a given ScaleOption.
	//Provide an AltKey to uniquely identify this feedback.
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit Feedback with Scaled Intensity and Duration",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void SubmitFeedbackWithIntensityDuration(UFeedbackFile* Feedback, const FString &AltKey, FRotationOption RotationOption, FScaleOption ScaleOption);

	//Register a given haptic feedback file under the specified key.
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Register Feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void RegisterFeedbackFile(const FString &Key, UFeedbackFile* Feedback);

	//Submit a haptic feeback pattern to the given device using a byte array.
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit Using Bytes",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void SubmitBytes(const FString &key, EPosition PositionEnum, const TArray<uint8>& InputBytes, int32 DurationInMilliSecs);

	//Submit a haptic feeback pattern to the given device using the DotPoint structure.
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit Using Dots",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void SubmitDots(const FString &key, EPosition PositionEnum, const TArray<FDotPoint> DotPoints, int32 DurationInMilliSecs);

	//Submit a haptic feeback pattern to the given device using the PathPoint structure.
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit Using Paths",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void SubmitPath(const FString &key, EPosition PositionEnum, const TArray<FPathPoint>PathPoints, int32 DurationInMilliSecs);

	//Check if there is any haptic feedback currently playing on any device.
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Is Any Feedback Currently Playing",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		bool IsAnythingPlaying();

	//Is the specified haptic feedback pattern currently playing
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Is Feedback Currently Playing",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		bool IsRegisteredPlaying(const FString &Key);

	//Is the given haptic device connected
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Is Device Connected",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		bool IsDeviceConnected(EPosition device);

	//Turn off all currently playing haptic feedback patterns
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Turn Off All Feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void TurnOffAllFeedback();

	//Turn off the specified haptic feedback pattern if it is playing;
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Turn Off Feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void TurnOffRegisteredFeedback(const FString &Key);

	//Enable haptic feedback
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Enable Feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void EnableHapticFeedback();

	//Disable haptic feedback
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Disable Feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void DisableHapticFeedback();

	//Toggle haptic feedback on/off
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Toggle Feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void ToggleHapticFeedback();

	//Helper function to compute rotation for a given collision.
	//Compute's the horizontal angle and vertical offset and returns a RotationOption for use with a haptic feedback file.
	//Assumes the origin of the component is in the center, and that the ForwardVector is the front of the vest.
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Project To Vest",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static FRotationOption ProjectToVest(FVector Location, UPrimitiveComponent* HitComponent, float HalfHeight = 50);

	//Helper function to compute rotation for a given collision.
	//Performs the same function as Project To Vest, but allows the Forward and Up vectors to be changed.
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Custom Project To Vest",
			Keywords = "bHaptics",
			AdvancedDisplay = "3"),
		Category = "bHaptics")
		static FRotationOption CustomProjectToVest(FVector Location, UPrimitiveComponent* HitComponent, float HalfHeight = 50, FVector UpVector = FVector::ZeroVector, FVector ForwardVector = FVector::ZeroVector);

private:
	static FCriticalSection m_Mutex;
	bool IsInitialised = false;
	FString Id;
};
