workspace "Akame"
    architecture "x64"
    startproject "imGui"
    configurations
    {
        "Debug",
        "Release"
    }
ComDir={}
ComDir["AkameCore"]="%{wks.location}/AkameCore/"

IncludeDir={}

IncludeDir["vendor"]="%{wks.location}/AkameCore/vendor"
IncludeDir["GLFW"]="%{IncludeDir.vendor}/glfw/include"
IncludeDir["imGui"]="%{IncludeDir.vendor}/imGui"
IncludeDir["assimp"]="%{IncludeDir.vendor}/assimp/include"
IncludeDir["AkameCore"]="%{wks.location}/AkameCore/Header Files"
IncludeDir["ECS"]="%{wks.location}/ECS/HeaderFiles"
IncludeDir["pugixml"]="%{IncludeDir.vendor}/pugixml/src"
IncludeDir["spdlog"]="%{IncludeDir.vendor}/spdlog/include"
IncludeDir["Physx"]=[[%{IncludeDir.vendor}/PhysX/physx/include;
%{IncludeDir.vendor}/PhysX/physx/source/physxextensions/src;
%{IncludeDir.vendor}/PhysX/pxshared/include;
%{IncludeDir.vendor}/PhysX/physx/source/foundation/include;
%{IncludeDir.vendor}/PhysX/physx/snippets]]


LibraryDir={}
LibraryDir["GLFW"]="%{IncludeDir.vendor}/glfw/src/%{cfg.buildcfg}"
LibraryDir["assimp"]="%{IncludeDir.vendor}/assimp/lib/%{cfg.buildcfg}"
LibraryDir["AkameCore"]="%{wks.location}/AkameCore/bin/%{cfg.buildcfg}"
LibraryDir["Physx"]="%{IncludeDir.vendor}/PhysX/physx/bin/win.x86_64.vc142.md/%{cfg.buildcfg}"
LibraryDir["imGui"]="%{IncludeDir.vendor}/imGui/%{cfg.system}/%{cfg.buildcfg}"
LibraryDir["pugixml"]="%{IncludeDir.vendor}/pugixml/src/bin/%{cfg.buildcfg}"

LiblinksRelease={
    "glfw3.lib",
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
    "glfw3.lib",
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
AllIncludeDir="%{IncludeDir.GLFW};%{IncludeDir.vendor};%{IncludeDir.imGui};%{IncludeDir.assimp};%{IncludeDir.ECS};%{IncludeDir.AkameCore};%{IncludeDir.pugixml};%{IncludeDir.spdlog};"..IncludeDir["Physx"]

AllDebugEnvPaths="PATH=$(SolutionDir)/AkameCore/vendor/PhysX/physx/bin/win.x86_64.vc142.md/%{cfg.buildcfg};$(SolutionDir)/AkameCore/vendor/assimp/bin/%{cfg.buildcfg}"

startproject "Test"

include "premakeconfig/AkameCore.lua"
include "premakeconfig/ECS.lua"
include "premakeconfig/AkameEngine.lua"

group "Dependencies"
    include "premakeconfig/imGui.lua"
    include "premakeconfig/Dependencies.lua"
group ""

group "Samples"
    include "premakeconfig/Planets.lua"
    include "premakeconfig/GeoSpatialData.lua"
    include "premakeconfig/Test.lua"
    include "premakeconfig/RenderStressTest.lua"
    include "premakeconfig/Model.lua"
    include "premakeconfig/snakeGame.lua"
    include "premakeconfig/RandomStuffTest.lua"
group ""






