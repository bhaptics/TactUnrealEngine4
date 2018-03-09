// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "HapticsManagerPrivatePCH.h"
#include "HapticsManager.h"
#include "SDK/hapticsManager.hpp"
#include "BhapticsUtilities.h"

#define LOCTEXT_NAMESPACE "FHapticsManagerModule"
bhaptics::HapticPlayer *bhaptics::HapticPlayer::hapticManager = 0;
void FHapticsManagerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if (BhapticsUtilities::Initialise())
	{
		FString temp = BhapticsUtilities::GetExecutablePath();

		if (!BhapticsUtilities::IsPlayerRunning())
		{
			UE_LOG(LogTemp, Log, TEXT("Player is not running"));

			if (BhapticsUtilities::IsPlayerInstalled())
			{
				UE_LOG(LogTemp, Log, TEXT("Player is installed - launching"));
				BhapticsUtilities::LaunchPlayer();
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Player is not Installed"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Player is running"));
		}

		BhapticsUtilities::Free();
	}
}

void FHapticsManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHapticsManagerModule, HapticsManager)