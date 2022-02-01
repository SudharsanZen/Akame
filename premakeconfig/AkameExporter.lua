project "Akame3DExporter"
        location"%{wks.location}/tools/AkameExporter"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
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
            string.explode(AllIncludeDir,";")
        }

		libdirs
		{
			LibraryDir["AkameCore"],
            "%{LibraryDir.GLFW}",
            "%{LibraryDir.Physx}",
            "%{LibraryDir.imGui}",
            "%{LibraryDir.assimp}",
            "%{LibraryDir.pugixml}"
		}

		links
		{
			"AkameCore.lib",
            "glfw3.lib",
            "opengl32.lib",
            "imGui.lib",
            "PhysX_64.lib",
            "PhysXCommon_64.lib",
            "PhysXCooking_64.lib",
            "PhysXFoundation_64.lib",
            "PhysXPvdSDK_static_64.lib",
            "PhysXExtensions_static_64.lib",
		    "pugixml.lib"
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
    filter { 'system:windows' }
        files { '%{wks.location}/resources.rc', "%{wks.location}/Assets/EngineAssets/AppIcons/**.ico"}
        vpaths { ['Resources/*'] = { '*.rc',  "%{wks.location}/Assets/EngineAssets/AppIcons/**.ico" }}
        filter {}