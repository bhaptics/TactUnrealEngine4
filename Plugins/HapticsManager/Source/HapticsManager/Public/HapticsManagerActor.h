//Copyright bHaptics Inc. 2017

#pragma once

#include "Engine.h"
#include "HapticStructures.h"
#include "GameFramework/Actor.h"
#include "HapticsManagerActor.generated.h"


UCLASS()
class HAPTICSMANAGER_API AHapticsManagerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHapticsManagerActor();

	static TArray<FHapticFeedback> ChangedFeedbacks;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform & Transform) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    virtual void Destroyed() override;

	//Names of files currently loaded by the HapticsManager
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
		TArray<FName> HapticFileNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
		FString HapticFileRootFolder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
		TArray<USceneComponent*> TactotFront;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
		TArray<USceneComponent*> TactotBack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
		TArray<USceneComponent*> TactosyLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
		TArray<USceneComponent*> TactosyRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
		TArray<USceneComponent*> Tactal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
		TArray<USceneComponent*> TactGloveLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
		TArray<USceneComponent*> TactGloveRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
		TArray<USceneComponent*> TactShoeLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
		TArray<USceneComponent*> TactShoeRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
		TArray<USceneComponent*> TactRacket;


	//Boolean to determine whether to load feedback files from the contents folder or the plugins directory.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
		bool UseProjectFeedbackFolder = true;

	//Submit a registered feedback using the file name as a key
    UFUNCTION(BlueprintCallable, 
        meta = (DisplayName = "Submit key", 
            Keywords = "bHaptics"), 
                Category = "bHaptics")
    void SubmitRegistered(const FString &Key);

	//Submit a registered feedback using the file name as a key, and scale the intensity and duration by given scales.
	 UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit key with Intensity and Duration",
            Keywords = "bHaptics"), 
                Category = "bHaptics")
    void SubmitRegisteredIntesityDuration(const FString &Key, float Intensity, float Duration);

	 //Submit a registered feedback using the file name as a key and transform by the given deltaX and deltaY.
	 //Also state if the feedback should continue when hitting an edge of the device or not.
	 UFUNCTION(BlueprintCallable,
		 meta = (DisplayName = "Submit key with Transform",
			 Keywords = "bHaptics"),
		 Category = "bHaptics")
		 void SubmitRegisteredTransform(const FString &Key, float DeltaX, float DeltaY, bool IsValueRotate);

	//Register a given feedback file from the given file path with the given key.
    UFUNCTION(BlueprintCallable,
        meta = (DisplayName = "Register feedback",
            Keywords = "bHaptics"),
        Category = "bHaptics")
    void RegisterFeeback(const FString &Key, const FString &FilePath);

	//Submit a haptic feeback pattern to the given device using a byte array.
	 UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit using Bytes",
            Keywords = "bHaptics"), 
                Category = "bHaptics")
    void SubmitBytes(const FString &key, EPosition PositionEnum,const TArray<uint8>& InputBytes, int32 DurationInMilliSecs);

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
    void SubmitPath(const FString &key, EPosition PositionEnum,const TArray<FPathPoint>PathPoints, int32 DurationInMilliSecs);

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

	 static void UpdateDisplayedFeedback(const char *ReceivedMessage);

	 void InitialiseDots(TArray<USceneComponent*> TactoSuitItem);

	 void VisualiseFeedback(FHapticFeedback Feedback, TArray<USceneComponent*> TactoSuitItem);

	 //For use in the UI to set the dots used in visualisation.
	 UFUNCTION(BlueprintCallable,
		 meta = (DisplayName = "Set TactSuit Variables",
			 Keywords = "bHaptics"),
		 Category = "bHaptics")
		 void SetTactoSuit(USceneComponent* SleeveLeft, USceneComponent* SleeveRight, USceneComponent* Head, USceneComponent* VestFront, USceneComponent* VestBack,
							USceneComponent* GloveLeft, USceneComponent* GloveRight, USceneComponent* ShoeLeft, USceneComponent* ShoeRight, USceneComponent* Racket);

	 UFUNCTION(BlueprintCallable,
		 meta = (DisplayName = "Reset Player Connection",
			 Keywords = "bHaptics"),
		 Category = "bHaptics")
		 void Reset();

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
		 bool MessagePlayed = false;
		 FString LoadFeedbackFiles(TArray<FString>& FilesOut);

};
