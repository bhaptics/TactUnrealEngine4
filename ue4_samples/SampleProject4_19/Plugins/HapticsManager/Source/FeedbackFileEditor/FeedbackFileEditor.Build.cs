// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.


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
				// ... add other private include paths required here ...
			}
			);
			
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "UnrealEd"
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
                "FeedbackFileEditor",
                "Json",
                "JsonUtilities",
                "Projects",
                "InputCore",
                "HapticsManager",
                "EditorStyle"
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

	}
}
