// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "HapticsManager.h"
#include "BhapticsLibrary.h"
#include "Misc/Paths.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FHapticsManagerModule"
void FHapticsManagerModule::StartupModule()
{
	FString BaseDir = IPluginManager::Get().FindPlugin("HapticsManager")->GetBaseDir();
	
	FString LibraryPath;
#if PLATFORM_WINDOWS
#if PLATFORM_32BITS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/HapticLibrary/DLLs/win32/haptic_library.dll"));
#else
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/HapticLibrary/DLLs/win64/haptic_library.dll"));
#endif
#elif PLATFORM_MAC
#endif // PLATFORM_WINDOWS

	UE_LOG(LogTemp, Log, TEXT("Path is %s"), *LibraryPath);
	
	HapticLibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;
	if (HapticLibraryHandle != nullptr)
	{
		BhapticsLibrary::SetLibraryLoaded();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Cannot find haptics_library.dll"));
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