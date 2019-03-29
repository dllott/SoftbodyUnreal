//code taken from Brandon Wilson's OpenCV plugin
using UnrealBuildTool;
using System.IO;

public class TestPlugin : ModuleRules
{
    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../../../ThirdParty/")); }
    }

    public TestPlugin(ReadOnlyTargetRules Target) : base(Target)
    {
        // Startard Module Dependencies
        //PublicDependencyModuleNames.AddRange(new string[] { "Core", "RHI", "RenderCore" });
        //PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "Slate", "SlateCore" });

        // Start OpenCV linking here!
        bool isLibrarySupported = false;

        // Create OpenCV Path 
        string LibFPath = Path.Combine(ThirdPartyPath, "SofaFrontend");

        // Get Library Path 
        string LibPath = "";
        bool isdebug = Target.Configuration == UnrealTargetConfiguration.Debug && Target.bDebugBuildsActuallyUseDebugCRT;
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            LibPath = Path.Combine(LibFPath, "Libraries", "Win64");
            isLibrarySupported = true;
        }
        else if (Target.Platform == UnrealTargetPlatform.Win32)
        {
            // TODO: add binaries for Win32
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            // TODO: add binaries for Mac
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            // TODO: add binaries for Linux
        }
        else
        {
            string Err = string.Format("{0} dedicated server is made to depend on {1}. We want to avoid this, please correct module dependencies.", Target.Platform.ToString(), this.ToString()); System.Console.WriteLine(Err);
        }

        if (isLibrarySupported)
        {
            //Add Include path 
            PublicIncludePaths.AddRange(new string[] { Path.Combine(LibFPath, "Includes") });

            // Add Library Path 
            PublicLibraryPaths.Add(LibPath);

            //Add Static Libraries
            PublicAdditionalLibraries.Add("SofaFrontendLite.lib");

            //Add Dynamic Libraries
            PublicDelayLoadDLLs.Add("SofaFrontendLite.dll");
        }

        Definitions.Add(string.Format("WITH_SOFAFRONTEND_BINDING={0}", isLibrarySupported ? 1 : 0));
    }
}