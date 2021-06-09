project "Opengl1"
    location"%{wks.location}/Opengl1/"
    kind "StaticLib"
    language "C++"
    targetdir "%{wks.location}/Opengl1/bin/%{cfg.buildcfg}"
    objdir "%{wks.location}/bin/Intermediate/%{cfg.buildcfg}/%{prj.name}"
    dependson{"ALL_BUILD_DEP","ECS"}
    debugdir "%{cfg.targetdir}"
    files
    {
        "%{prj.location}/Header Files/**.h",
        "%{prj.location}/Source Files/**.cpp",
    }

    includedirs
    {
        string.explode(AllIncludeDir,";")
    }

    libdirs
    {
        "%{LibraryDir.GLFW}",
        "%{LibraryDir.Physx}",
        "%{LibraryDir.imGui}",
        "%{LibraryDir.assimp}"
    }

    links
    {
        "glfw3.lib",
        "opengl32.lib",
        "imGui.lib",
        "PhysX_64.lib",
        "PhysXCommon_64.lib",
        "PhysXCooking_64.lib",
        "PhysXFoundation_64.lib",
        "PhysXPvdSDK_static_64.lib",
        "PhysXExtensions_static_64.lib"

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
