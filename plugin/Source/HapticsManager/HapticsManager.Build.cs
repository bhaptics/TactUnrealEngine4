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
                "Json",
                "JsonUtilities",
                "Projects",
                "InputCore",
				"DeveloperSettings",
                "HapticLibrary"
				// ... add private dependencies that you statically link with here ...	
			}
			);

        
        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

    }

}
