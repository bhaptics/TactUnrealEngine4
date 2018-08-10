//Copyright bHaptics Inc. 2018

#pragma once

#if (ENGINE_MAJOR_VERSION >= 4) && (ENGINE_MINOR_VERSION >= 16)
#include "CoreMinimal.h"
#endif

#include "HapticStructures.h"
#include "Engine/Engine.h"

class HAPTICSMANAGER_API BhapticsLibrary
{
public:
	BhapticsLibrary();
	~BhapticsLibrary();

	static bool Initialise();

	static void Free();
	
	static void Lib_RegisterConnection(FString Id);

	static void Lib_UnregisterConnection(FString Id);

	static void Lib_RegisterFeedback(FString Key, FString ProjectJson);

	static void Lib_SubmitRegistered(FString Key);

	static void Lib_SubmitRegistered(FString Key, FString AltKey, FScaleOption ScaleOpt, FRotationOption RotOption);

	static void Lib_Submit(FString Key, EPosition Pos, TArray<uint8> MotorBytes, int DurationMillis);

	static void Lib_Submit(FString Key, EPosition Pos, TArray<FDotPoint> Points, int DurationMillis);

	static void Lib_Submit(FString Key, EPosition Pos, TArray<FPathPoint> Points, int DurationMillis);

	static bool Lib_IsFeedbackRegistered(FString key);

	static bool Lib_IsPlaying();

	static bool Lib_IsPlaying(FString Key);

	static void Lib_TurnOff();

	static void Lib_TurnOff(FString Key);

	static void Lib_EnableFeedback();

	static void Lib_DisableFeedback();

	static void Lib_ToggleFeedback();

	static bool Lib_IsDevicePlaying(EPosition Pos);

	static TArray<FHapticFeedback> Lib_GetResponseStatus();

private:
	static bool IsInitialised;
};
