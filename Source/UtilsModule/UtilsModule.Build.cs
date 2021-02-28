// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UtilsModule : ModuleRules
{
	public UtilsModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
        //OptimizeCode = CodeOptimization.Never;
    }
}
