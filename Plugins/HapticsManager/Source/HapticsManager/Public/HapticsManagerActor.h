//Copyright bHaptics Inc. 2018

#pragma once

#include "Engine.h"
#include "HapticStructures.h"
#include "GameFramework/Actor.h"
#include "FeedbackFile.h"
#include "HapticsManagerActor.generated.h"


UCLASS()
class HAPTICSMANAGER_API AHapticsManagerActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHapticsManagerActor();

	TArray<FHapticFeedback> ChangedFeedbacks;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform & Transform) override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	TArray<USceneComponent*> TactotFront;

	TArray<USceneComponent*> TactotBack;

	TArray<USceneComponent*> TactosyLeft;

	TArray<USceneComponent*> TactosyRight;

	TArray<USceneComponent*> Tactal;

	TArray<USceneComponent*> TactGloveLeft;

	TArray<USceneComponent*> TactGloveRight;

	TArray<USceneComponent*> TactShoeLeft;

	TArray<USceneComponent*> TactShoeRight;

	TArray<USceneComponent*> TactRacket;

	//For use in the UI to set the dots used in visualisation.
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName = "Set TactSuit Variables",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void SetTactSuit(USceneComponent* SleeveLeft, USceneComponent* SleeveRight, USceneComponent* Head, USceneComponent* VestFront, USceneComponent* VestBack,
			USceneComponent* GloveLeft, USceneComponent* GloveRight, USceneComponent* ShoeLeft, USceneComponent* ShoeRight, USceneComponent* Racket);

private:
	static FCriticalSection m_Mutex;
	bool IsTicking = false;
	void UpdateFeedback();
	void VisualiseFeedback(FHapticFeedback Feedback, TArray<USceneComponent*> TactoSuitItem);
	void InitialiseDots(TArray<USceneComponent*> TactoSuitItem);
	FString Id;

};