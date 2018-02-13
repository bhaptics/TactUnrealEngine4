// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HapticsManagerPrivatePCH.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "Engine.h"
#include "FeedbackFile.generated.h"

UCLASS(BlueprintType, hidecategories = (Object))
class HAPTICSMANAGER_API UFeedbackFile : public UObject
{
	GENERATED_BODY()

private:
	

public :
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "TextAsset")
		FString Key;

	FJsonObject Project;
};
