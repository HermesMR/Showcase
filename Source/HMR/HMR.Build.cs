// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HMR : ModuleRules
{
	public HMR(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UtilsModule", "StatsModule", "AbilitiesModule" });
        //OptimizeCode = CodeOptimization.Never;
    }
}
