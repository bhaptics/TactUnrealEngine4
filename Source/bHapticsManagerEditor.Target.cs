// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class bHapticsManagerEditorTarget : TargetRules
{
	public bHapticsManagerEditorTarget(TargetInfo Target)
	{
		Type = TargetType.Editor;
        ExtraModuleNames.AddRange(new string[] { "bHapticsManager" });
    }

    //
    // TargetRules interface.
    //
#if VERSION_BELOW_16
    public override void SetupBinaries(TargetInfo Target,
    ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
    ref List<string> OutExtraModuleNames
    )
    {
        OutExtraModuleNames.AddRange(new string[] { "bHapticsManager" });
    }
#endif
}
