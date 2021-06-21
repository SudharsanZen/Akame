workspace "GameEngine"
    architecture "x64"
    startproject "imGui"
    configurations
    {
        "Debug",
        "Release"
    }

IncludeDir={}
IncludeDir["GLFW"]="%{wks.location}/glfw/include"
IncludeDir["vendor"]="%{wks.location}/Opengl1/vendor"
IncludeDir["imGui"]="%{wks.location}/Opengl1/vendor/imGui"
IncludeDir["assimp"]="%{wks.location}/Opengl1/vendor/assimp/include"
IncludeDir["Engine"]="%{wks.location}/Opengl1/Header Files"
IncludeDir["ECS"]="%{wks.location}/ECS/HeaderFiles"
IncludeDir["pugixml"]="%{wks.location}/Opengl1/vendor/pugixml/src"
IncludeDir["spdlog"]="%{wks.location}/Opengl1/vendor/spdlog/include"
IncludeDir["Physx"]=[[%{wks.location}/Opengl1/vendor/PhysX/physx/include;
%{wks.location}/Opengl1/vendor/PhysX/physx/source/physxextensions/src;
%{wks.location}/Opengl1/vendor/PhysX/pxshared/include;
%{wks.location}/Opengl1/vendor/PhysX/physx/source/foundation/include;
%{wks.location}/Opengl1/vendor/PhysX/physx/snippets]]


LibraryDir={}
LibraryDir["GLFW"]="%{wks.location}/glfw/src/%{cfg.buildcfg}"
LibraryDir["assimp"]="%{wks.location}/Opengl1/vendor/assimp/lib/%{cfg.buildcfg}"
LibraryDir["Engine"]="%{wks.location}/Opengl1/bin/%{cfg.buildcfg}"
LibraryDir["Physx"]="%{wks.location}/Opengl1/vendor/PhysX/physx/bin/win.x86_64.vc142.md/%{cfg.buildcfg}"
LibraryDir["imGui"]="%{wks.location}/Opengl1/vendor/imGui/%{cfg.system}/%{cfg.buildcfg}"
LibraryDir["pugixml"]="%{wks.location}/Opengl1/vendor/pugixml/src/bin/%{cfg.buildcfg}"

LiblinksRelease={
    "glfw3.lib",
    "opengl32.lib",
    "imGui.lib",
    "assimp-vc142-mt.lib",
    "Opengl1.lib",

    "PhysX_64.lib",
    "PhysXCommon_64.lib",
    "PhysXCooking_64.lib",
    "PhysXFoundation_64.lib",
    "PhysXPvdSDK_static_64.lib",
    "PhysXExtensions_static_64.lib"
    }

LiblinksDebug={
    "glfw3.lib",
    "opengl32.lib",
    "imGui.lib",
    "assimp-vc142-mtd.lib",
    "Opengl1.lib",

    "PhysX_64.lib",
    "PhysXCommon_64.lib",
    "PhysXCooking_64.lib",
    "PhysXFoundation_64.lib",
    "PhysXPvdSDK_static_64.lib",
    "PhysXExtensions_static_64.lib"
    }
AllIncludeDir="%{IncludeDir.GLFW};%{IncludeDir.vendor};%{IncludeDir.imGui};%{IncludeDir.assimp};%{IncludeDir.ECS};%{IncludeDir.Engine};%{IncludeDir.pugixml};%{IncludeDir.spdlog};"..IncludeDir["Physx"]

AllDebugEnvPaths="PATH=$(SolutionDir)/Opengl1/vendor/PhysX/physx/bin/win.x86_64.vc142.md/%{cfg.buildcfg};$(SolutionDir)/Opengl1/vendor/assimp/bin/%{cfg.buildcfg}"

startproject "Test"

include "premakeConf/Opengl1.lua"
include "premakeConf/ECS.lua"

group "Dependencies"
    include "premakeConf/imGui.lua"
    include "premakeConf/Dependencies.lua"
group ""

group "Samples"
    include "premakeConf/Planets.lua"
    include "premakeConf/GeoSpatialData.lua"
    include "premakeConf/Test.lua"
group ""






