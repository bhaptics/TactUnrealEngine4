// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class HapticsManager : ModuleRules
{
    public string GetUProjectPath()
    {
        //Change this according to your module's relative location to your project file. If there is any better way to do this I'm interested!
        //Assuming Source/ThirdParty/YourLib/
        return Directory.GetParent(ModuleDirectory).Parent.ToString();
    }

    private void CopyToBinaries(string Filepath, TargetInfo Target)
    {
        string binariesDir = Path.Combine(GetUProjectPath(), "Binaries", Target.Platform.ToString());
        string filename = Path.GetFileName(Filepath);

        if (!Directory.Exists(binariesDir))
            Directory.CreateDirectory(binariesDir);

        if (!File.Exists(Path.Combine(binariesDir, filename)))
            File.Copy(Filepath, Path.Combine(binariesDir, filename), true);
    }

    public HapticsManager(TargetInfo Target)
	{

        //UEBuildConfiguration.bForceEnableExceptions = true;

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

        //PublicAdditionalLibraries.Add(ModuleDirectory + "/bHapticUtility.dll");
        //RuntimeDependencies.Add(new RunTimeDependency(ModuleDirectory + "/bHapticUtility.dll"));

        CopyToBinaries(ModuleDirectory + "/bHapticUtility.dll",Target);
	}
}
