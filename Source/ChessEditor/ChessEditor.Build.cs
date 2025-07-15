// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ChessEditor : ModuleRules
{
	public ChessEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Chess",
				"Core",
				"CoreUObject",
				"Engine",
                "UnrealEd",
                "Slate",
                "SlateCore",
                "EditorStyle",
                "InputCore",
                "InputBindingEditor",
            }
		);

		PrivateIncludePaths.AddRange(
			new string[] {
				"ChessEditor/Private",
			}
		);

		var DynamicModuleNames = new string[] {
			"LevelEditor",
			"PropertyEditor",
		};

		foreach (var Name in DynamicModuleNames)
		{
			PrivateIncludePathModuleNames.Add(Name);
			DynamicallyLoadedModuleNames.Add(Name);
		}
	}
}