//Copyright bHaptics Inc. 2017-2019

using UnrealBuildTool;
using System.Collections.Generic;

public class bHapticsManagerTarget : TargetRules
{
	public bHapticsManagerTarget(TargetInfo Target): base(Target)
	{
		Type = TargetType.Game;
        //UEBuildConfiguration.bForceEnableExceptions = true;
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
		OutExtraModuleNames.AddRange( new string[] { "bHapticsManager" } );
	}
#endif

}
