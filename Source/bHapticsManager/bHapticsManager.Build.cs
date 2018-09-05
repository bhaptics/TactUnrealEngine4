// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class bHapticsManager : ModuleRules
{
#if VERSION_BELOW_16
    public bHapticsManager(TargetInfo Target)
    {
#else
    public bHapticsManager(ReadOnlyTargetRules Target) : base(Target)
    {
#endif
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
