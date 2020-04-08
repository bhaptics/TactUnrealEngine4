// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class SampleProject4_19Target : TargetRules
{
	public SampleProject4_19Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

        /////////////// KNOWN ISSUE 
        ///   https://forums.unrealengine.com/development-discussion/c-gameplay-programming/1348011-can-t-package-for-32-bit-failed-to-produce-item-game-win32-shipping-exe
        /// To Fix : use VS 2015 compiler
        /// https://community.gamedev.tv/t/unable-to-create-new-project-in-unreal-engine-following-course/105531
        WindowsPlatform.Compiler = WindowsCompiler.VisualStudio2015;

        ExtraModuleNames.AddRange( new string[] { "SampleProject4_19" } );
	}
}
