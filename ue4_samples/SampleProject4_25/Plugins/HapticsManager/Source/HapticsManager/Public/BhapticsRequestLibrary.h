// Copyright 2019, bHaptics Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HapticStructures.h"
#include "FeedbackFile.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "BhapticsRequestLibrary.generated.h"
/**
 * 
 */
UCLASS()
class HAPTICSMANAGER_API UBhapticsRequestLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Initialize bHaptics",
			Keywords = "bHaptics"),
			Category = "bHaptics")
		static void Initialize();

	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit Feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static void SubmitFeedback(UFeedbackFile* Feedback);

	// Deprecated
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit Feedback with Transform",
			Keywords = "bHaptics",
			DeprecatedFunction,
			DeprecationMessage = "Now managed through  `Submit Feedback wih Options`..",
			AdvancedDisplay = "AltKey"),
		Category = "bHaptics")
		static void SubmitFeedbackWithTransform(UFeedbackFile* Feedback, const FString& AltKey, FRotationOption Option, bool UseAltKey = false);

	// Deprecated
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit Feedback with Scaled Intensity and Duration",
			Keywords = "bHaptics",
			DeprecatedFunction,
			DeprecationMessage = "Now managed through `Submit Feedback wih Options`.",
			AdvancedDisplay = "AltKey"),
		Category = "bHaptics")
		static void SubmitFeedbackWithIntensityDuration(UFeedbackFile* Feedback, const FString& AltKey, FRotationOption RotationOption, FScaleOption ScaleOption, bool UseAltKey = false);

	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit Feedback with Options",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static void SubmitFeedbackWithOptions(
			UFeedbackFile* Feedback, 
			const FString& Key, 
			FRotationOption RotationOption, 
			FScaleOption ScaleOption);

	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit Feedback with Scale Option",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static void SubmitFeedbackWithScaleOption(
			UFeedbackFile* Feedback, 
			FScaleOption ScaleOption);

	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit Feedback Reflected - Works with Arm, Hand and Foot",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static void SubmitFeedbackReflected(
			UFeedbackFile* Feedback,
			FScaleOption ScaleOption);

	//Register a given haptic feedback file under the specified key.
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Register Feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static void RegisterFeedbackFile(const FString& Key, UFeedbackFile* Feedback);

	//Submit a haptic feeback pattern to the given device using a byte array.
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit Using Bytes",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static void SubmitBytes(const FString& key, EPosition PositionEnum, const TArray<uint8>& InputBytes, int32 DurationInMilliSecs);

	//Submit a haptic feeback pattern to the given device using the DotPoint structure.
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit Using Dots",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static void SubmitDots(const FString& key, EPosition PositionEnum, const TArray<FDotPoint> DotPoints, int32 DurationInMilliSecs);

	//Submit a haptic feeback pattern to the given device using the PathPoint structure.
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit Using Paths",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static void SubmitPath(const FString& key, EPosition PositionEnum, const TArray<FPathPoint>PathPoints, int32 DurationInMilliSecs);

	//Check if there is any haptic feedback currently playing on any device.
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Is Any Feedback Currently Playing",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static bool IsAnythingPlaying();

	//Is the specified haptic feedback pattern currently playing using the AltKey value.
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Is Feedback Currently Playing",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static bool IsRegisteredPlaying(const FString& Key);

	//Is the specified haptic feedback pattern currently playing.
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Is Feedback File Currently Playing",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static bool IsRegisteredFilePlaying(UFeedbackFile* Feedback);

	//Is the given haptic device connected
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Is Device Connected",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static bool IsDeviceConnected(EPosition device);

	//Turn off all currently playing haptic feedback patterns
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Turn Off All Feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static void TurnOffAllFeedback();

	//Turn off the specified haptic feedback pattern if it is playing;
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Turn Off Feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static void TurnOffRegisteredFeedback(const FString& Key);

	//Turn off the specified haptic feedback pattern if it is playing;
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Turn Off Feedback File",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static void TurnOffRegisteredFeedbackFile(UFeedbackFile* Feedback);

	//Enable haptic feedback
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Enable Feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static void EnableHapticFeedback();

	//Disable haptic feedback
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Disable Feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static void DisableHapticFeedback();

	//Toggle haptic feedback on/off
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Toggle Feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static void ToggleHapticFeedback();

	//Toggle haptic feedback on/off
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Get Status",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static TArray<uint8> GetResponseStatus(EPosition Pos);

	//Helper function to compute rotation for a given collision.
	//Compute's the horizontal angle and vertical offset and returns a RotationOption for use with a haptic feedback file.
	//Assumes the origin of the component is in the center, and that the ForwardVector is the front of the vest.
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Project To Vest With Player Collision Object",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static FRotationOption ProjectToVest(FVector ContactLocation, UPrimitiveComponent* PlayerComponent, float HalfHeight = 50);
	
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Project To Vest With Player Location/Position",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		static FRotationOption ProjectToVestLocation(FVector ContactLocation, FVector PlayerLocation, FRotator PlayerRotation);

	//Helper function to compute rotation for a given collision.
	//Performs the same function as Project To Vest, but allows the Forward and Up vectors to be changed.
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Custom Project To Vest",
			Keywords = "bHaptics",
			AdvancedDisplay = "3"),
		Category = "bHaptics")
		static FRotationOption CustomProjectToVest(FVector ContactLocation, UPrimitiveComponent* PlayerComponent, float HalfHeight = 50, FVector UpVector = FVector::ZeroVector, FVector ForwardVector = FVector::ZeroVector);
};
