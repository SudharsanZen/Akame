newoption {
    trigger     = "ak_root",
    value       = "path",
    description = "root folder of akame github project"
}
newoption {
    trigger     = "solution_name",
    value       = "name",
    description = "Visual Studio solution name"
}
newoption {
    trigger     = "wks_location",
    value       = "path",
    description = "directory in which the solution files should be generated to"
}
AkameRoot = _OPTIONS["ak_root"] or "../.."
ComDir={}
ComDir["AkameCore"]="%{AkameRoot}/AkameCore/"

IncludeDir={}

IncludeDir["vendor"]="%{AkameRoot}/AkameCore/vendor"
IncludeDir["glad"]="%{IncludeDir.vendor}/glad-akame-"
IncludeDir["json"]="%{IncludeDir.vendor}/single_include/nlohmann"
IncludeDir["GLFW"]="%{IncludeDir.vendor}/glfw/include"
IncludeDir["imGui"]="%{IncludeDir.vendor}/imGui"
IncludeDir["assimp"]="%{IncludeDir.vendor}/assimp/include"
IncludeDir["AkameCore"]="%{AkameRoot}/AkameCore/Header Files"
IncludeDir["ECS"]="%{AkameRoot}/ECS/HeaderFiles"
IncludeDir["pugixml"]="%{IncludeDir.vendor}/pugixml/src"
IncludeDir["spdlog"]="%{IncludeDir.vendor}/spdlog/include"
IncludeDir["Physx"]=[[%{IncludeDir.vendor}/PhysX/physx/include;
%{IncludeDir.vendor}/PhysX/physx/source/physxextensions/src;
%{IncludeDir.vendor}/PhysX/pxshared/include;
%{IncludeDir.vendor}/PhysX/physx/source/foundation/include;
%{IncludeDir.vendor}/PhysX/physx/snippets]]


LibraryDir={}
LibraryDir["GLFW"]="%{IncludeDir.vendor}/glfw/builds/src/%{cfg.buildcfg}"
LibraryDir["assimp"]="%{IncludeDir.vendor}/assimp/lib/%{cfg.buildcfg}"
LibraryDir["AkameCore"]="%{AkameRoot}/AkameCore/bin/%{cfg.buildcfg}"
LibraryDir["Physx"]="%{IncludeDir.vendor}/PhysX/physx/bin/win.x86_64.vc142.md/%{cfg.buildcfg}"
LibraryDir["imGui"]="%{IncludeDir.vendor}/imGui/%{cfg.system}/%{cfg.buildcfg}"
LibraryDir["pugixml"]="%{IncludeDir.vendor}/pugixml/src/bin/%{cfg.buildcfg}"

LiblinksRelease={
    "glfw3dll.lib",
    "opengl32.lib",
    "imGui.lib",
    "assimp-vc142-mt.lib",
    "AkameCore.lib",

    "PhysX_64.lib",
    "PhysXCommon_64.lib",
    "PhysXCooking_64.lib",
    "PhysXFoundation_64.lib",
    "PhysXPvdSDK_static_64.lib",
    "PhysXExtensions_static_64.lib"
    }

LiblinksDebug={
    "glfw3dll.lib",
    "opengl32.lib",
    "imGui.lib",
    "assimp-vc142-mtd.lib",
    "AkameCore.lib",

    "PhysX_64.lib",
    "PhysXCommon_64.lib",
    "PhysXCooking_64.lib",
    "PhysXFoundation_64.lib",
    "PhysXPvdSDK_static_64.lib",
    "PhysXExtensions_static_64.lib"
    }
AllIncludeDir="%{IncludeDir.GLFW};%{IncludeDir.json};%{IncludeDir.glad};%{IncludeDir.vendor};%{IncludeDir.imGui};%{IncludeDir.assimp};%{IncludeDir.ECS};%{IncludeDir.AkameCore};%{IncludeDir.pugixml};%{IncludeDir.spdlog};"..IncludeDir["Physx"]

workspace (_OPTIONS["solution_name"])
    location (_OPTIONS["wks_location"])
    architecture "x64"
    configurations
    {
        "Release"
    } 
project "Scriptables"
        location ("%{_OPTIONS.wks_location}/Scriptables")
        kind "SharedLib"
        language "C++"
        cppdialect "C++17"
        targetdir "%{wks.location}/bin/%{cfg.buildcfg}/%{cfg.architecture}/Samples/"
        objdir "%{wks.location}/bin/Intermediate/%{cfg.buildcfg}/%{prj.name}"

        includedirs 
        {
            IncludeDir["AkameCore"],IncludeDir["ECS"],IncludeDir["vendor"]
        }

		libdirs
		{
			LibraryDir["AkameCore"]
		}
        files
        {
            "%{prj.location}/**.h",
            "%{prj.location}/**.cpp"
        }
		links
		{
			"AkameCore.lib"
		}

        filter "configurations:Release"
            defines{"NDEBUG","AK_DLL"}
            optimize "On"

