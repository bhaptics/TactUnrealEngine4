//Copyright bHaptics Inc. 2017-2019

using System.IO;
using UnrealBuildTool;

public class HapticsManagerLibrary : ModuleRules
{
    public HapticsManagerLibrary(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            // Add the import library
            PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "x64", "Release"));
            PublicAdditionalLibraries.Add("HapticLibrary.lib");
            
            // Delay-load the DLL, so we can load it from the right place first
            PublicDelayLoadDLLs.Add("HapticLibrary.dll");
        }
        else if (Target.Platform == UnrealTargetPlatform.Win32)
        {
            // Add the import library
            PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "x86", "Release"));
            PublicAdditionalLibraries.Add("HapticLibrary.lib");

            // Delay-load the DLL, so we can load it from the right place first
            PublicDelayLoadDLLs.Add("HapticLibrary.dll");
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "HapticLibrary.dylib"));
        }
    }
}
