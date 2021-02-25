// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HapticStructures.h"

#include "CoreMinimal.h"

/**
 * 
 */
class HAPTICSMANAGER_API BhapticsUtils
{
public:
	BhapticsUtils();
	~BhapticsUtils();


	static FString PositionEnumToString(EPosition Position);
	static FString PositionEnumToDeviceString(EPosition Position);
};
