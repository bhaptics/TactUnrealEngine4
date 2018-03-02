// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "Engine.h"
#include "HapticStructures.h"
#include "FeedbackFile.generated.h"

UCLASS(BlueprintType, hidecategories = (Object))
class HAPTICSMANAGER_API UFeedbackFile : public UObject
{
	GENERATED_BODY()

public :
	//Key used to identify the haptic feedback effect in the bHaptics Player.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FeedbackFile")
	FString Key;
	
	//JSON String to be sent to the Player
	UPROPERTY()
	FString ProjectString;

	//Unique ID for each feedbackFile
	UPROPERTY()
	FGuid Id;

	//Device(s) on which the haptic feedback will be played.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FeedbackFile")
		FString Device;

	//Duration of the haptic feedback effect (in seconds)
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FeedbackFile")
		float Duration;
};

