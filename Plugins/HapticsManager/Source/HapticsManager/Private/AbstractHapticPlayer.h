// Copyright bHaptics Inc. 2017-2019

#pragma once
#include "HapticStructures.h"
#include "CoreMinimal.h"

/**
 * 
 */
class AbstractHapticPlayer
{
public:
	virtual bool Initialise()=0;
	virtual void Destroy()=0;

	virtual void RegisterFeedbackProject(FString Key, FString ProjectJson) =0;
	virtual void SubmitRegisteredFeedback(FString Key) = 0;
	virtual void SubmitRegisteredFeedback(FString Key, FString AltKey, FScaleOption ScaleOpt, FRotationOption RotOption)=0;
	virtual void SubmitFeedback(FString Key, EPosition Pos, TArray<uint8> MotorBytes, int DurationMillis)=0;
	virtual void SubmitFeedback(FString Key, EPosition Pos, TArray<FDotPoint> Points, int DurationMillis)=0;
	virtual void SubmitFeedback(FString Key, EPosition Pos, TArray<FPathPoint> Points, int DurationMillis)=0;
	virtual void TurnOff()=0;
	virtual void TurnOff(FString Key) = 0;


	virtual bool IsRegistered(FString Key)=0;
	virtual bool IsPlaying()=0;
	virtual bool IsPlaying(FString Key) = 0;
	virtual bool IsDeviceConnected(EPosition Pos)=0;

	virtual TArray<FHapticFeedback> GetResponseStatus() =0;

	virtual void EnableFeedback()=0;
	virtual void DisableFeedback()=0;
	virtual void ToggleFeedback()=0;

	virtual void startScan()=0;
	virtual void stopScan()=0;
};
