project "Akame3DExporter"
        location"%{wks.location}/tools/AkameExporter"
        kind "ConsoleApp"
        language "C++"
        targetdir "%{wks.location}/bin/%{cfg.buildcfg}/%{cfg.architecture}/tools/"
        objdir "%{wks.location}/bin/Intermediate/%{cfg.buildcfg}/%{prj.name}"
        dependson{"imGui","glfw","AkameCore","assimp"}
        debugdir "%{wks.location}/bin/%{cfg.buildcfg}/%{cfg.architecture}/tools/"
        files
        {
            "%{prj.location}/**.h",
            "%{prj.location}/**.cpp",
        }

        includedirs
        {
            IncludeDir["AkameCore"],IncludeDir["ECS"],IncludeDir["vendor"],IncludeDir["assimp"]
        }

		libdirs
		{
			LibraryDir["AkameCore"], LibraryDir["assimp"]
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
		links
		{
			"assimp-vc142-mtd.lib"
		}

     filter "configurations:Release"
        defines{"NDEBUG"}
        optimize "On"
		links
		{
			"assimp-vc142-mt.lib",
		}
