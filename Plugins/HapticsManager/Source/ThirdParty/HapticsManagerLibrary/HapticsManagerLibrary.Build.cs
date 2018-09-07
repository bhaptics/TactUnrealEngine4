//Copyright bHaptics Inc. 2018

//#define VERSION_BELOW_16
//#define VERSION_BELOW_19

using System.IO;
using UnrealBuildTool;

public class HapticsManagerLibrary : ModuleRules
{
#if VERSION_BELOW_16
    public HapticsManagerLibrary(TargetInfo Target)
    {
#else
    public HapticsManagerLibrary(ReadOnlyTargetRules Target) : base(Target)
    {
#endif
        Type = ModuleType.External;

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            // Add the import library
            PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "x64", "Release"));
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
