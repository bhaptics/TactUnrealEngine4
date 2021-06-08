// Copyright 2015~2021 bHaptics Inc. All Rights Reserved.

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
            });
        
        PrivateIncludePaths.AddRange(
            new string[] {
                //"HapticsManager/Private",
            });

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
			}
			);

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {

            }
            );
    }

}
