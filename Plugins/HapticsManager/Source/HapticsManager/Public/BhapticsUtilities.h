// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "CoreMinimal.h"

/**
 * 
 */
class HAPTICSMANAGER_API BhapticsUtilities
{
public:
	BhapticsUtilities();
	~BhapticsUtilities();

	static FString GetExecutablePath();

	static bool IsPlayerInstalled();

	static bool IsPlayerRunning();

	static void LaunchPlayer();

	static void Init();

	static void Free();

private:
	static bool IsInitialised;
	static FString ExecutableFilePath;
};
