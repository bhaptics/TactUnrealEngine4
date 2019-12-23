//Copyright bHaptics Inc. 2017-2019

using System.IO;
using UnrealBuildTool;

public class HapticLibrary : ModuleRules
{
    public HapticLibrary(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            // Add the import library
            PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "../../DLLs/win64"));
            PublicAdditionalLibraries.Add("haptic_library.lib");
            
            // Delay-load the DLL, so we can load it from the right place first
            PublicDelayLoadDLLs.Add("haptic_library.dll");
        }
        else if (Target.Platform == UnrealTargetPlatform.Win32)
        {
            // Add the import library
            PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "../../DLLs/win32"));
            PublicAdditionalLibraries.Add("haptic_library.lib");

            // Delay-load the DLL, so we can load it from the right place first
            PublicDelayLoadDLLs.Add("haptic_library.dll");
        }
    }
}
