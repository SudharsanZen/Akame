
project "GeoSpatialData"
    location"%{wks.location}/GeoSpatialData/"
    kind "ConsoleApp"
    language "C++"
    targetdir "%{wks.location}/GeoSpatialData/bin/%{cfg.buildcfg}"
    objdir "%{wks.location}/bin/Intermediate/%{cfg.buildcfg}/%{prj.name}"
    debugdir "%{cfg.targetdir}"
    dependson{"imGui","glfw","Opengl1"}
    files
    {
        "%{prj.location}/Header Files/**.h",
        "%{prj.location}/Source Files/**.cpp",
    }

    includedirs
    {

            IncludeDir["Engine"],IncludeDir["ECS"],IncludeDir["vendor"]
    }
    debugenvs
    {
        AllDebugEnvPaths
    }
    libdirs
    {
        LibraryDir
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
