// Copyright bHaptics Inc. 2017-2019

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbstractHapticPlayer.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAbstractHapticPlayer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IAbstractHapticPlayer
{
	GENERATED_BODY()

public:
	void Initialise();
	void Destroy();

	void RegisterFeedback();
	void SubmitRegistered();
	void Submit();
	void TurnOff();

	bool IsRegistered();
	bool IsPlaying();
	bool IsDeviceConnected();

	void EnableFeedback();
	void DisableFeedback();
	void ToggleFeedback();

	void startScan();
	void stopScan();
};
