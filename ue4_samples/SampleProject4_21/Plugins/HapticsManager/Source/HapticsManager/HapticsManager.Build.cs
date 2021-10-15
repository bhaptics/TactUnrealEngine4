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
                "HapticLibrary"
			}
			);

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {

            }
            );



        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "../HapticLibrary/DLLs/win64/haptic_library.dll"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Win32)
        {
            RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "../HapticLibrary/DLLs/win32/haptic_library.dll"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", Path.Combine(PluginPath, "HapticsManager_APL.xml")));
        }
    }

}
