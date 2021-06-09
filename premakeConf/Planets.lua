project "Planets"
    location"%{wks.location}/Planets/"
    kind "ConsoleApp"
    language "C++"
    targetdir "%{wks.location}/Planets/bin/%{cfg.buildcfg}"
    objdir "%{wks.location}/bin/Intermediate/%{cfg.buildcfg}/%{prj.name}"
    dependson{"imGui","glfw","Opengl1"}
    debugdir "%{cfg.targetdir}"
    files
    {
        "%{prj.location}/Header Files/**.h",
        "%{prj.location}/Source Files/**.cpp",
    }

        includedirs
    {
        string.explode(AllIncludeDir,";")
    }

    libdirs
    {
        LibraryDir

    }
    debugenvs
    {
        AllDebugEnvPaths
    }
    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"
		links
		{
			LiblinksDebug
		}
    filter "configurations:Release"
        defines{"NDEBUG"}
        optimize "On"
		links
		{
			LiblinksRelease
		}
