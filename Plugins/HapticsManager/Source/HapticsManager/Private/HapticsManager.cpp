// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "HapticsManager.h"
#include "BhapticsLibrary.h"
#include "Misc/Paths.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FHapticsManagerModule"
void FHapticsManagerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("HapticsManager")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
#if PLATFORM_WINDOWS
#if PLATFORM_32BITS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("DLLs/win32/haptic_library.dll"));
#else
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("DLLs/win64/haptic_library.dll"));
#endif
#elif PLATFORM_MAC
#endif // PLATFORM_WINDOWS

	HapticLibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;
	if (HapticLibraryHandle != nullptr)
	{
		BhapticsLibrary::SetLibraryLoaded();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Cannot find Library"));
	}
}

void FHapticsManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (HapticLibraryHandle != nullptr)
	{
		BhapticsLibrary::Free();
		FPlatformProcess::FreeDllHandle(HapticLibraryHandle);
	}
	HapticLibraryHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHapticsManagerModule, HapticsManager)