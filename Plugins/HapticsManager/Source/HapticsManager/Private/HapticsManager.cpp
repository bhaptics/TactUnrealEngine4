// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "HapticsManager.h"
#include "BhapticsUtilities.h"
#include "BhapticsLibrary.h"

#define LOCTEXT_NAMESPACE "FHapticsManagerModule"
void FHapticsManagerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if (BhapticsUtilities::Initialise())
	{
#if (ENGINE_MINOR_VERSION <= 18)
		FString ConfigPath = *FPaths::GameContentDir();
#else
		FString ConfigPath = *FPaths::ProjectContentDir();
#endif

		ConfigPath.Append("/ConfigFiles/HapticPlayer.txt");
		if (FPaths::FileExists(ConfigPath)) {
			FString ExeLocation;
			FFileHelper::LoadFileToString(ExeLocation, *ConfigPath);
			if (FPaths::FileExists(ExeLocation)) {
				if (!BhapticsUtilities::IsExternalPlayerRunning(ExeLocation))
				{
					auto Handle = FPlatformProcess::CreateProc(*ExeLocation, nullptr, true, false, false, nullptr, 0, nullptr, nullptr);
				}
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Could not launch: %s."), *ExeLocation);
			}
		}
		else
		{

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
		}
		BhapticsUtilities::Free();
	}

	//BhapticsLibrary::Initialise();

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("HapticsManager")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
#if PLATFORM_WINDOWS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("DLLs/HapticLibrary.dll"));
#elif PLATFORM_MAC
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/HapticsManagerLibrary/Mac/Release/libExampleLibrary.dylib"));
#endif // PLATFORM_WINDOWS

	HapticLibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	BhapticsLibrary::Initialise();
}

void FHapticsManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	BhapticsLibrary::Free();
	// Free the dll handle
	FPlatformProcess::FreeDllHandle(HapticLibraryHandle);
	HapticLibraryHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHapticsManagerModule, HapticsManager)