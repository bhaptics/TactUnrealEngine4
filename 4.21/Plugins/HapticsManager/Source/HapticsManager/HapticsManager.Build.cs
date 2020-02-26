// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class HapticsManager : ModuleRules
{
    public HapticsManager(ReadOnlyTargetRules Target) : base (Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivatePCHHeaderFile = "Public/HapticsManager.h";

        PublicIncludePaths.AddRange(
			new string[] {
                //"HapticsManager/Public"
				// ... add public include paths required here ...
			}
			);
				
		PrivateIncludePaths.AddRange(
			new string[] {
                //"HapticsManager/Private",
				// ... add other private include paths required here ...
			}
			);

        PrivateIncludePathModuleNames.AddRange(
            new string[] {
                        "Settings",
                        "Launch",
            }
            );
        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "HapticsManager",
                //"System",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "HapticsManager",
                "Json",
                "JsonUtilities",
                "Projects",
                "InputCore",
                "HapticLibrary"
                //"Sockets",
				// ... add private dependencies that you statically link with here ...	
			}
			);

        
        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "../../DLLs/win64/haptic_library.dll"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Win32)
        {
            RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "../../DLLs/win32/haptic_library.dll"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "HapticsManager_APL.xml"));
        }

    }

}
