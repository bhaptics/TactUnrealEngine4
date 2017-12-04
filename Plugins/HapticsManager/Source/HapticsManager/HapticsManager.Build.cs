// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class HapticsManager : ModuleRules
{
    public HapticsManager(ReadOnlyTargetRules Target): base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
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
                "Projects"
                //"Sockets",
				// ... add private dependencies that you statically link with here ...	
			}
			);

        //string BinaryLocation = Directory.GetParent(ModuleDirectory).ToString();
        //PublicIncludePaths.Add(BinaryLocation + @"DLLs\Include");
        //PublicIncludePaths.Add(BinaryLocation + @"Utility\Include");
        
        //PublicAdditionalLibraries.Add(BinaryLocation + @"\Utility\Libraries\bHapticUtility.lib");//D:\bHapticsManager\bHapticsRelease - Gitlab\Plugins\HapticsManager\Binaries\Win64\bHapticUtility.lib
        
        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

	}
}
