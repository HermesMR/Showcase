// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class StatsModule : ModuleRules
{
	public StatsModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UtilsModule" });
        //OptimizeCode = CodeOptimization.Never;
    }
}
