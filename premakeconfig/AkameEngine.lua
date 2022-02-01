project "AkameEngine"
        location"%{wks.location}/Akame"
        kind "ConsoleApp"
        language "C++"
        targetdir "%{wks.location}/bin/%{cfg.buildcfg}/%{cfg.architecture}"
        objdir "%{wks.location}/bin/Intermediate/%{cfg.buildcfg}/%{prj.name}"
        dependson{"imGui","glfw","AkameCore"}
        debugdir "%{wks.location}/bin/%{cfg.buildcfg}/%{cfg.architecture}"
        files
        {
            "%{prj.location}/Header Files/**.h",
            "%{prj.location}/Source/**.cpp",
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
        filter { 'system:windows' }
            files { '%{wks.location}/resources.rc', "%{wks.location}/Assets/EngineAssets/AppIcons/**.ico"}
            vpaths { ['Resources/*'] = { '*.rc',  "%{wks.location}/Assets/EngineAssets/AppIcons/**.ico" }}
            filter {}