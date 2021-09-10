
project "GeoSpatialData"
    location"%{wks.location}/Samples/GeoSpatialData/"
    kind "ConsoleApp"
    language "C++"
    targetdir "%{wks.location}/Samples/GeoSpatialData/bin/%{cfg.buildcfg}"
    objdir "%{wks.location}/bin/Intermediate/%{cfg.buildcfg}/%{prj.name}"
    debugdir "%{cfg.targetdir}"
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
