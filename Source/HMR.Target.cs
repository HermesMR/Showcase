// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class HMRTarget : TargetRules
{
	public HMRTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("HMR");
        ExtraModuleNames.Add("StatsModule");
        ExtraModuleNames.Add("AbilitiesModule");
        ExtraModuleNames.Add("UtilsModule");
    }
}
