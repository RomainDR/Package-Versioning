// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VersionningPackage : ModuleRules
{
	public VersionningPackage(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[]
			{
				// ... add public include paths required here ...
			}
		);


		PrivateIncludePaths.AddRange(
			new string[]
			{
				// ... add other private include paths required here ...
			}
		);


		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core", "Slate"
				// ... add other public dependencies that you statically link with here ...
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"InputCore",
				"Json"
				// ... autres runtime-only modules ici ...
			}
		);

		if (Target.bBuildEditor)
			PrivateDependencyModuleNames.AddRange(
				new[]
				{
					"EditorFramework",
					"UnrealEd",
					"LevelEditor",
					"InteractiveToolsFramework",
					"EditorInteractiveToolsFramework",
					"ToolMenus",
					"EditorStyle"
					// ... autres modules Editor-only ici ...
				}
			);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
	}
}