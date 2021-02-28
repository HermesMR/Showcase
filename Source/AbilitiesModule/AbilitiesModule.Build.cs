// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AbilitiesModule : ModuleRules
{
	public AbilitiesModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UtilsModule", "StatsModule" });
        //OptimizeCode = CodeOptimization.Never;
    }
}
