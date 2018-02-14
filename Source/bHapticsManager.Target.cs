//Copyright bHaptics Inc. 2017

using UnrealBuildTool;
using System.Collections.Generic;

public class bHapticsManagerTarget : TargetRules
{
	public bHapticsManagerTarget(TargetInfo Target)
	{
		Type = TargetType.Game;
        UEBuildConfiguration.bForceEnableExceptions = true;
    }

    //
    // TargetRules interface.
    //
	public override void SetupBinaries(TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
		OutExtraModuleNames.AddRange( new string[] { "bHapticsManager" } );
	}

}
