// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CommandLineTarget : TargetRules
{
	public CommandLineTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("CommandLine");

		BuildEnvironment = TargetBuildEnvironment.Shared;

		// below are the variables which worked for me
		PreBuildSteps.Add("echo Project Directory: \"$(ProjectDir)\"");
		PreBuildSteps.Add("echo Engine Directory: \"$(EngineDir)\"");
		PreBuildSteps.Add("echo Target Name: \"$(TargetName)\"");

		if (Platform == UnrealTargetPlatform.Mac
		    || Platform == UnrealTargetPlatform.Linux
		    || Platform == UnrealTargetPlatform.LinuxArm64)
		{
			// bash script-wrapper that I run before UE Builds
			PreBuildSteps.Add("$(ProjectDir)/Automation/pre_build.sh $(ProjectDir)/Source");

			// bash script-wrapper that I run after UE Builds
			PostBuildSteps.Add("$(ProjectDir)/Automation/post_build.sh");
		}

		if (Platform == UnrealTargetPlatform.Win64)
		{
			// bash script-wrapper that I run before UE Builds
			PreBuildSteps.Add("$(ProjectDir)\\Automation\\pre_build.bat \"$(ProjectDir)\\Source\"");

			// bash script-wrapper that I run after UE Builds
			PostBuildSteps.Add("$(ProjectDir)\\Automation\\post_build.bat");
		}
	}
}