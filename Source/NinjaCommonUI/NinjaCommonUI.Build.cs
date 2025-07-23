// Ninja Bear Studio Inc., all rights reserved.
using UnrealBuildTool;

public class NinjaCommonUI : ModuleRules
{
    public NinjaCommonUI(ReadOnlyTargetRules target) : base(target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new []
            {
	            "CommonInput",
	            "CommonUI",
	            "Core",
	            "GameplayTags",
	            "InputCore",
	            "UMG"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new []
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
    }
}