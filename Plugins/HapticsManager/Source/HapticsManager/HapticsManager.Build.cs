// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#define VERSION_BELOW_16

using System;
using System.IO;
using UnrealBuildTool;

public class HapticsManager : ModuleRules
{

    public HapticsManager(TargetInfo Target)
	{
        //PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
			new string[] {
                "HapticsManager/Public"
				// ... add public include paths required here ...
			}
			);
				
		PrivateIncludePaths.AddRange(
			new string[] {
                "HapticsManager/Private",
				// ... add other private include paths required here ...
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
                "HapticsManagerLibrary",
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
