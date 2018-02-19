// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "HapticStructures.h"
#include "FeedbackFile.h"
#include "AllowWindowsPlatformTypes.h"
#include "SDK/hapticsManager.hpp"
#include "HideWindowsPlatformTypes.h"
#include "HapticManagerComponent.generated.h"


UCLASS( ClassGroup=(bHaptics), meta=(BlueprintSpawnableComponent) )
class HAPTICSMANAGER_API UHapticManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHapticManagerComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	TArray<FHapticFeedback> ChangedFeedbacks;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//Names of files currently loaded by the HapticsManager
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
		TArray<FName> HapticFileNames;

	//Root folder storing the files saved in the HapticFileNames Array.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
	FString HapticFileRootFolder;

	//Boolean to determine whether to load feedback files from the project's contents folder or the plugins directory.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
		bool UseProjectFeedbackFolder = true;

	//Folder in the Contents directory to search for the haptic feedback files.
	//The full path is not needed, only its location relative to the Contents directory.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
		FString ProjectFeedbackFolder = "HapticsManager/Feedback";

	//Submit a registered feedback using the file name as a key
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit Key",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void SubmitKey(UFeedbackFile* Feedback);

	//Submit a registered feedback using the file name as a key and a given RotationOption.
	//This call will only rotate vest feedback files, with other devices being kept the same.
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit Key with Transform",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void SubmitKeyWithTransform(UFeedbackFile* Feedback, const FString &AltKey, FRotationOption Option);

	//Submit a registered feedback using the file name as a key, and scale the intensity and duration by given ScaleOption.
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit Key with Scaled Intensity and Duration",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void SubmitKeyWithIntensityDuration(UFeedbackFile* Feedback, const FString &AltKey, FRotationOption RotationOption, FScaleOption ScaleOption);

	//Register a given feedback file from the given file path with the given key.
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Register Feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void RegisterFeedback(const FString &Key, UFeedbackFile* Feedback);

	//Submit a haptic feeback pattern to the given device using a byte array.
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit using Bytes",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void SubmitBytes(const FString &key, EPosition PositionEnum, const TArray<uint8>& InputBytes, int32 DurationInMilliSecs);

	//Submit a haptic feeback pattern to the given device using the DotPoint structure.
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit using Dots",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void SubmitDots(const FString &key, EPosition PositionEnum, const TArray<FDotPoint> DotPoints, int32 DurationInMilliSecs);

	//Submit a haptic feeback pattern to the given device using the PathPoint structure.
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit using Paths",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void SubmitPath(const FString &key, EPosition PositionEnum, const TArray<FPathPoint>PathPoints, int32 DurationInMilliSecs);

	//Check if there is any feedback patterns currently playing on any device.
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Is feedback currently playing",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		bool IsAnythingPlaying();

	//Is the specified feedback pattern currently playing
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Is the feedback currently playing",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		bool IsRegisteredPlaying(const FString &Key);

	//Is the given device connected
	UFUNCTION(BlueprintPure,
		meta = (DisplayName = "Is the device connected",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		bool IsDeviceConnected(EPosition device);

	//Turn off all currently playing feedback patterns
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Turn off all feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void TurnOffAllFeedback();

	//Turn off the specified feedback pattern if it is playing;
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Turn off feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void TurnOffRegisteredFeedback(const FString &Key);

	//Enable haptic feedback
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Enable Feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void EnableFeedback();

	//Disable haptic feedback
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Disable Feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void DisableFeedback();

	//Toggle haptic feedback on/off
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Toggle Feedback",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void ToggleFeedback();

private:
	static FCriticalSection m_Mutex;
	static FString HapticFileRootFolderStatic;
	bool IsInitialised = false;
	bool IsTicking = false;
	void Reset();
	
};
