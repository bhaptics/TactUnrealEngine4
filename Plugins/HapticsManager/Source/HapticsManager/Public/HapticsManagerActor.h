//Copyright bHaptics Inc. 2017

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
		TArray<FName> HapticFileNames;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptics")
		bool UseProjectFeedbackFolder = false;

    UFUNCTION(BlueprintCallable, 
        meta = (DisplayName = "Submit registered feedback with key", 
            Keywords = "bHaptics"), 
                Category = "bHaptics")
    void SubmitRegistered(const FString &Key);

	 UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit registered feedback with Intensity and Duration",
            Keywords = "bHaptics"), 
                Category = "bHaptics")
    void SubmitRegisteredIntesityDuration(const FString &Key, float Intensity, float Duration);

    UFUNCTION(BlueprintCallable,
        meta = (DisplayName = "Register Haptic feedback",
            Keywords = "bHaptics"),
        Category = "bHaptics")
    void RegisterFeeback(const FString &Key, const FString &FilePath);

	 UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit feedback using Bytes",
            Keywords = "bHaptics"), 
                Category = "bHaptics")
    void SubmitBytes(const FString &key, EPosition PositionEnum,const TArray<uint8>& InputBytes, int32 DurationInMilliSecs);

	 UFUNCTION(BlueprintCallable,
		 meta = (DisplayName = "Submit feedback using Dots",
			 Keywords = "bHaptics"),
		 Category = "bHaptics")
	void SubmitDots(const FString &key, EPosition PositionEnum, const TArray<FDotPoint> DotPoints, int32 DurationInMilliSecs);

	 UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Submit feedback using Paths",
            Keywords = "bHaptics"), 
                Category = "bHaptics")
    void SubmitPath(const FString &key, EPosition PositionEnum,const TArray<FPathPoint>PathPoints, int32 DurationInMilliSecs);

	 UFUNCTION(BlueprintPure,
		 meta = (DisplayName = "Is any feedback currently playing",
			 Keywords = "bHaptics"),
		 Category = "bHaptics")
	 bool IsAnythingPlaying();
	 
	 UFUNCTION(BlueprintPure,
		 meta = (DisplayName = "Is the specified registered feedback currently playing",
			 Keywords = "bHaptics"),
		 Category = "bHaptics")
	 bool IsRegisteredPlaying(const FString &Key);

	 UFUNCTION(BlueprintPure,
		 meta = (DisplayName = "Is the device connected",
			 Keywords = "bHaptics"),
		 Category = "bHaptics")
		 bool IsDeviceConnected(EPosition device);

	 UFUNCTION(BlueprintCallable,
		 meta = (DisplayName = "Turn off all feedback",
			 Keywords = "bHaptics"),
		 Category = "bHaptics")
	 void TurnOffAllFeedback();

	 UFUNCTION(BlueprintCallable,
		 meta = (DisplayName = "Turn off a specified feedback",
			 Keywords = "bHaptics"),
		 Category = "bHaptics")
	 void TurnOffRegisteredFeedback(const FString &Key);

	 static void UpdateDisplayedFeedback(const char *ReceivedMessage);

	 void InitialiseDots(TArray<USceneComponent*> TactoSuitItem);

	 void VisualiseFeedback(FHapticFeedback Feedback, TArray<USceneComponent*> TactoSuitItem);

	 UFUNCTION(BlueprintCallable,
		 meta = (DisplayName = "Set the TactSuit Variables",
			 Keywords = "bHaptics"),
		 Category = "bHaptics")
		 void SetTactoSuit(USceneComponent* SleeveLeft, USceneComponent* SleeveRight, USceneComponent* Head, USceneComponent* VestFront, USceneComponent* VestBack,
							USceneComponent* GloveLeft, USceneComponent* GloveRight, USceneComponent* ShoeLeft, USceneComponent* ShoeRight, USceneComponent* Racket);

	 UFUNCTION(BlueprintCallable,
		 meta = (DisplayName = "Reset Player Connection",
			 Keywords = "bHaptics"),
		 Category = "bHaptics")
		 void Reset();

	 UFUNCTION(BlueprintCallable,
		 meta = (DisplayName = "Enable Feedback",
			 Keywords = "bHaptics"),
		 Category = "bHaptics")
	 void EnableFeedback();

	 UFUNCTION(BlueprintCallable,
		 meta = (DisplayName = "Disable Feedback",
			 Keywords = "bHaptics"),
		 Category = "bHaptics")
	 void DisableFeedback();

	 UFUNCTION(BlueprintCallable,
		 meta = (DisplayName = "Toggle Feedback On/Off",
			 Keywords = "bHaptics"),
		 Category = "bHaptics")
	 void ToggleFeedback();

	 private:
		 static FCriticalSection m_Mutex;
		 bool MessagePlayed = false;
		 FString LoadFeedbackFiles(TArray<FString>& FilesOut);
};
