// Copyright 2015~2021 bHaptics Inc. All Rights Reserved.


using System;
using System.IO;
using UnrealBuildTool;

public class FeedbackFileEditor : ModuleRules
{ 
    public FeedbackFileEditor(ReadOnlyTargetRules Target) : base (Target) 
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivatePCHHeaderFile = "Private/FeedbackFileEditor.h";

        PrivateIncludePaths.AddRange(
            new string[] {
                "FeedbackFileEditor/Private",
            }
            );
        
        PublicDependencyModuleNames.AddRange(
            new string[] 
            {
                "Core",
                "UnrealEd"
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
                "HapticsManager",
                "EditorStyle"
            }
            );

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            }
            );
    }
}
