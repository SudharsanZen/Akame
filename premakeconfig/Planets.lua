project "Planets"
    location"%{wks.location}/Samples/Planets/"
    kind "ConsoleApp"
    language "C++"
    targetdir "%{wks.location}/Samples/Planets/bin/%{cfg.buildcfg}"
    objdir "%{wks.location}/bin/Intermediate/%{cfg.buildcfg}/%{prj.name}"
    dependson{"imGui","glfw","AkameCore"}
    debugdir "%{cfg.targetdir}"
    files
    {
        "%{prj.location}/Header Files/**.h",
        "%{prj.location}/Source Files/**.cpp",
    }

	includedirs
    {

            IncludeDir["AkameCore"],IncludeDir["ECS"],IncludeDir["vendor"]
    }

    libdirs
	{
		LibraryDir["AkameCore"]
	}

    links
	{
		"AkameCore.lib"
	}
    debugenvs
    {
        AllDebugEnvPaths
    }
    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"

    filter "configurations:Release"
        defines{"NDEBUG"}
        optimize "On"
