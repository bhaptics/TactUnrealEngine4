//Copyright bHaptics Inc. 2017

#pragma once
#include "Engine.h"

class HAPTICSMANAGER_API BhapticsUtilities
{
public:
	BhapticsUtilities();
	~BhapticsUtilities();

	static FString GetExecutablePath();

	static bool IsPlayerInstalled();

	static bool IsPlayerRunning();

	static void LaunchPlayer();

	static bool Initialise();

	static void Free();

private:
	static bool IsInitialised;
	static FString ExecutableFilePath;
};
