project "RenderStressTest"
        location"%{wks.location}/Samples/RenderStressTest"
        kind "ConsoleApp"
        language "C++"
        targetdir "%{prj.location}/bin/%{cfg.buildcfg}"
        objdir "%{wks.location}/bin/Intermediate/%{cfg.buildcfg}/%{prj.name}"
        dependson{"imGui","glfw","AkameCore"}
        debugdir "%{prj.location}/bin"
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