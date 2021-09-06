//Copyright bHaptics Inc. 2018

#pragma once

#include "CoreMinimal.h"
#include "HapticStructures.h"
#include "GameFramework/Actor.h"
#include "HapticsManagerActor.generated.h"


UCLASS()
class HAPTICSMANAGER_API AHapticsManagerActor : public AActor
{
	GENERATED_BODY()

public:
	AHapticsManagerActor();

	TArray<USceneComponent*> TactotFront;

	TArray<USceneComponent*> TactotBack;

	TArray<USceneComponent*> TactosyLeft;

	TArray<USceneComponent*> TactosyRight;

	TArray<USceneComponent*> Tactal;

	TArray<USceneComponent*> TactGloveLeft;

	TArray<USceneComponent*> TactGloveRight;

	TArray<USceneComponent*> TactShoeLeft;

	TArray<USceneComponent*> TactShoeRight;

	//For use in the UI to set the dots used in visualisation.
	UFUNCTION(BlueprintCallable,
		meta = (DeprecatedFunction, DisplayName = "Set TactSuit Variables",
			Keywords = "bHaptics"),
		Category = "bHaptics")
		void SetTactSuit(USceneComponent* SleeveLeft, USceneComponent* SleeveRight, USceneComponent* Head, USceneComponent* VestFront, USceneComponent* VestBack,
			USceneComponent* GloveLeft, USceneComponent* GloveRight, USceneComponent* ShoeLeft, USceneComponent* ShoeRight);

};