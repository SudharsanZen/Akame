project "AkameCore"
    location"%{ComDir.AkameCore}"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    targetdir "%{ComDir.AkameCore}/bin/%{cfg.buildcfg}"
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
        "%{LibraryDir.assimp}",
		"%{LibraryDir.pugixml}"
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
        "PhysXExtensions_static_64.lib",
		"pugixml.lib"

    }

    debugenvs
    {
        AllDebugEnvPaths
    }
    filter "configurations:Debug"
        defines {"DEBUG","AK_DLL","AK_EXPORT"}
        symbols "On"
		links
		{
			"assimp-vc142-mtd.lib"
		}

    filter "configurations:Release"
        defines{"NDEBUG","AK_DLL","AK_EXPORT"}
        optimize "On"
		links
		{
			"assimp-vc142-mt.lib",
		}
