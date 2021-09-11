
project "GeoSpatialData"
    location"%{wks.location}/Samples/GeoSpatialData/"
    kind "ConsoleApp"
    language "C++"
    targetdir "%{wks.location}/bin/%{cfg.buildcfg}/%{cfg.architecture}/Samples/"
    objdir "%{wks.location}/bin/Intermediate/%{cfg.buildcfg}/%{prj.name}"
    debugdir "%{wks.location}/bin/%{cfg.buildcfg}/%{cfg.architecture}/Samples/"
    dependson{"imGui","glfw","AkameCore"}
    files
    {
        "%{prj.location}/Header Files/**.h",
        "%{prj.location}/Source Files/**.cpp",
    }

    includedirs
    {

            IncludeDir["AkameCore"],IncludeDir["ECS"],IncludeDir["vendor"]
    }
    debugenvs
    {
        AllDebugEnvPaths
    }

	libdirs
	{
		LibraryDir["AkameCore"]
	}

   links
	{
		"AkameCore.lib"
	}


    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"

    filter "configurations:Release"
        defines{"NDEBUG"}
        optimize "On"
