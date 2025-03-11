// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Dedicated : ModuleRules
{
	public Dedicated(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks", "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Niagara", "OnlineSubsystem", "OnlineSubsystemUtils", "NetCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "AnimGraphRuntime", "NNEOnnxEditor", "ChaosVehicles","ChaosVehiclesEngine", "CinematicCamera" });
		// if (Target.bBuildEditor)
		// {
		// 	PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
		// }
	}
}
