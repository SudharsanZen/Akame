project "Model"
        location"%{wks.location}/Samples/Model"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
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
            defines {"DEBUG","AK_DLL"}
            symbols "On"

        filter "configurations:Release"
            defines{"NDEBUG","AK_DLL"}
            optimize "On"
        filter { 'system:windows' }
            files { '%{wks.location}/resources.rc', "%{wks.location}/Assets/EngineAssets/AppIcons/**.ico"}
            vpaths { ['Resources/*'] = { '*.rc',  "%{wks.location}/Assets/EngineAssets/AppIcons/**.ico" }}
            filter {}