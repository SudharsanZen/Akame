project "Test"
        location"%{wks.location}/Samples/Test"
        kind "ConsoleApp"
        language "C++"
        targetdir "%{wks.location}/bin/%{cfg.buildcfg}/%{cfg.architecture}/Samples/"
        objdir "%{wks.location}/bin/Intermediate/%{cfg.buildcfg}/%{prj.name}"
        dependson{"imGui","glfw","AkameCore"}
        debugdir "%{wks.location}/bin/%{cfg.buildcfg}/%{cfg.architecture}/Samples/"
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
