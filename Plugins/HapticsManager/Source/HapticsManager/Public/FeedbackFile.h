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

private:
	

public :
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FeedbackFile")
	FString Key;
	
	UPROPERTY()
	FString ProjectString;

	UPROPERTY()
	FGuid Id;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FeedbackFile")
		FString Device;
};

