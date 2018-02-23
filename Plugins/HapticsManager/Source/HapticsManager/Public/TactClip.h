// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "HapticStructures.h"
#include "FeedbackFile.h"
#include "TactClip.generated.h"

UCLASS(BlueprintType, hidecategories = (Object))
class HAPTICSMANAGER_API UTactClip : public UObject
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Haptics")
	TArray<FPathPoint> Path;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Haptics")
	float Intensity;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Haptics")
		float Duration;

	int Playbacktype;

	int MovingPattern;
};
