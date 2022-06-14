// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Leverplayer : ModuleRules
{
	public Leverplayer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "OnlineSubsystem", "OnlineSubsystemUtils" });

        PublicIncludePaths.AddRange(new string[]
        {
            "Leverplayer"
        });

    }
}
