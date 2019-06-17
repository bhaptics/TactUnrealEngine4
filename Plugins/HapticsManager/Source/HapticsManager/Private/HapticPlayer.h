// Copyright bHaptics Inc. 2017-2019

#pragma once
#include "AbstractHapticPlayer.h"
#include "CoreMinimal.h"

/**
 * 
 */
class HapticPlayer : public AbstractHapticPlayer
{
public:
	bool Initialise();
	void Destroy();

	void RegisterFeedbackProject(FString Key, FString ProjectJson);
	void SubmitRegisteredFeedback(FString Key);
	void SubmitRegisteredFeedback(FString Key, FString AltKey, FScaleOption ScaleOpt, FRotationOption RotOption);
	void SubmitFeedback(FString Key, EPosition Pos, TArray<uint8> MotorBytes, int DurationMillis);
	void SubmitFeedback(FString Key, EPosition Pos, TArray<FDotPoint> Points, int DurationMillis);
	void SubmitFeedback(FString Key, EPosition Pos, TArray<FPathPoint> Points, int DurationMillis);
	void TurnOff();
	void TurnOff(FString key);

	bool IsRegistered(FString key);
	bool IsPlaying();
	bool IsPlaying(FString Key);
	bool IsDeviceConnected(EPosition Pos);

	TArray<FHapticFeedback> GetResponseStatus();

	void EnableFeedback();
	void DisableFeedback();
	void ToggleFeedback();

	void startScan();
	void stopScan();

private:
	static bool IsInitialised;
	static bool Success;
	static FProcHandle Handle;
};
