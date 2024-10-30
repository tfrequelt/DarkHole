// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BlackHolePower0 : ModuleRules
{
	public BlackHolePower0(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
