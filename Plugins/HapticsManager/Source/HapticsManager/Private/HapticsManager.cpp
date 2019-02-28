// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "HapticsManager.h"
#include "BhapticsLibrary.h"
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
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("DLLs/x86/HapticLibrary.dll"));
#else
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("DLLs/HapticLibrary.dll"));
#endif
#elif PLATFORM_MAC
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/HapticsManagerLibrary/Mac/Release/libExampleLibrary.dylib"));
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

//bool FHapticsManagerModule::SupportsDynamicReloading()
//{
//	return true;
//}

/*
bool FHapticsManagerModule::HandleSettingsSaved()
{
	UCustomGameSettings* Settings = GetMutableDefault<UCustomGameSettings>();
	bool ResaveSettings = false;

	// You can put any validation code in here and resave the settings in case an invalid
	// value has been entered

	if (ResaveSettings)
	{
		Settings->SaveConfig();
	}

	return true;
}

void FHapticsManagerModule::RegisterSettings()
{
	// Registering some settings is just a matter of exposing the default UObject of
	// your desired class, feel free to add here all those settings you want to expose
	// to your LDs or artists.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Create the new category
		ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");

		SettingsContainer->DescribeCategory("CustomSettings",
			LOCTEXT("RuntimeWDCategoryName", "CustomSettings"),
			LOCTEXT("RuntimeWDCategoryDescription", "Game configuration for the CustomSettings game module"));

		// Register the settings
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "CustomSettings", "General",
			LOCTEXT("RuntimeGeneralSettingsName", "General"),
			LOCTEXT("RuntimeGeneralSettingsDescription", "Base configuration for our game module"),
			GetMutableDefault<UCustomGameSettings>()
			);

		// Register the save handler to your settings, you might want to use it to
		// validate those or just act to settings changes.
		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FCustomSettingsModule::HandleSettingsSaved);
		}
	}
}

void FHapticsManagerModuleUnregisterSettings()
{
	// Ensure to unregister all of your registered settings here, hot-reload would
	// otherwise yield unexpected results.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "CustomSettings", "General");
	}
}

*/


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHapticsManagerModule, HapticsManager)