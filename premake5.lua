workspace "GameEngine"
    architecture "x64"
    startproject "imGui"
    configurations
    {
        "Debug",
        "Release"
    }
startproject "Opengl1"

project "imGui"
    location"opengl1/vendor/imGui"
    kind "StaticLib"
    language "C++"
    targetdir "%{prj.location}/%{cfg.system}/%{cfg.buildcfg}"
    dependson{"glfw"}
    files
    {
        "%{prj.location}/*.h",
        "%{prj.location}/*.c",
        "%{prj.location}/*.cpp",
        "opengl1/vendor/glad/glad.c",
        "%{prj.location}/backends/imgui_impl_opengl3.cpp",
        "%{prj.location}/backends/imgui_impl_opengl3.h",
        "%{prj.location}/backends/imgui_impl_glfw.h",
        "%{prj.location}/backends/imgui_impl_glfw.cpp"
    }

    includedirs
    {
        "glfw/include",
        "opengl1/vendor",
        "%{prj.location}"
    }

    libdirs
    {
        "glfw/src/Debug"        
    }

    links
    {
        "glfw3",
        "opengl32"
    }
    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"
    
    filter "configurations:Release"
        defines{"NDEBUG"}
        optimize "On" 


project "Opengl1"
    location"Opengl1/"
    kind "ConsoleApp"
    language "C++"
    targetdir "Opengl1/bin/"
    objdir "bin/intermediate"
    dependson{"imGui","glfw"}
    files
    {
        "%{prj.location}/Header Files/**.h",
        "%{prj.location}/Source Files/**.cpp",
    }

    includedirs
    {
        "Opengl1/vendor/PhysX/physx/include",
        "Opengl1/vendor/PhysX/physx/source/physxextensions/src",
        "Opengl1/vendor/PhysX/pxshared/include",
        "Opengl1/vendor/PhysX/physx/source/foundation/include",


        "glfw/include",
        "Opengl1/vendor",
        "%{prj.location}/Header Files",
        "%{prj.location}/vendor",
        "%{prj.location}/vendor/imGui",
        "%{prj.location}/vendor/assimp/include",
        "ECS/HeaderFiles"
    }

    libdirs
    {
        "glfw/src/Debug",
        "Opengl1/vendor/PhysX/physx/bin/win.x86_64.vc142.mt/%{cfg.buildcfg}",
        "%{prj.location}/vendor/imGui/windows/Debug",
        "%{prj.location}/vendor/assimp/lib/Debug"
    }

    links
    {
        "glfw3.lib",
        "opengl32.lib",
        "imGui.lib",
        "assimp-vc142-mtd.lib",
        
        "PhysX_64.lib",
        "PhysXCommon_64.lib",
        "PhysXCooking_64.lib",
        "PhysXFoundation_64.lib",
        "PhysXExtensions_static_64.lib"

    }

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"
    
    filter "configurations:Release"
        defines{"NDEBUG"}
        optimize "On" 


project "ECS"
    location"ECS"
    kind "StaticLib"
    language "C++"
    targetdir "%{prj.location}/%{cfg.system}/%{cfg.buildcfg}"
    files
    {
        "%{prj.location}/HeaderFiles/**.h",
        "%{prj.location}/SourceFiles/**.cpp",
    }

    includedirs
    {
        "ECS/HeaderFiles",
    }

    libdirs
    {
     "ECS/windows/Debug"
    }

    filter "configurations:Debug"
    defines {"DEBUG"}
    symbols "On"

    filter "configurations:Release"
    defines{"NDEBUG"}
    optimize "On" 

    

externalproject "GLFW"
    location "glfw/src/"
    kind "StaticLib"


project "Planets"
    location"Planets/"
    kind "ConsoleApp"
    language "C++"
    targetdir "Planets/bin/"
    objdir "bin/intermediate"
    dependson{"imGui","glfw","Opengl1"}
    files
    {
        "%{prj.location}/Header Files/**.h",
        "%{prj.location}/Source Files/**.cpp",
    }

        includedirs
    {
        "glfw/include",
        "Opengl1/vendor",
        "Opengl1/Header Files",
        "Opengl1/vendor",
        "Opengl1/vendor/imGui",
        "Opengl1/vendor/assimp/include",
        "ECS/HeaderFiles"
    }

    libdirs
    {
        "glfw/src/Debug",
        "Opengl1/vendor/imGui/windows/Debug",
        "Opengl1/vendor/assimp/lib/Debug",
        "Opengl1/bin"
    }

    links
    {
        "glfw3.lib",
        "opengl32.lib",
        "imGui.lib",
        "assimp-vc142-mtd.lib",
        "Opengl1.lib"
    }
    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"
    
    filter "configurations:Release"
        defines{"NDEBUG"}
        optimize "On" 


project "GeoSpatialData"
    location"GeoSpatialData/"
    kind "ConsoleApp"
    language "C++"
    targetdir "GeoSpatialData/bin/"
    objdir "bin/intermediate"
    
    dependson{"imGui","glfw","Opengl1"}
    files
    {
        "%{prj.location}/Header Files/**.h",
        "%{prj.location}/Source Files/**.cpp",
    }

        includedirs
    {
        "glfw/include",
        "Opengl1/vendor",
        "Opengl1/Header Files",
        "Opengl1/vendor",
        "Opengl1/vendor/imGui",
        "Opengl1/vendor/assimp/include",
        "ECS/HeaderFiles"
    }

    libdirs
    {
        "glfw/src/Debug",
        "Opengl1/vendor/imGui/windows/Debug",
        "Opengl1/vendor/assimp/lib/Debug",
        "Opengl1/bin"
    }

    links
    {
        "glfw3.lib",
        "opengl32.lib",
        "imGui.lib",
        "assimp-vc142-mtd.lib",
        "Opengl1.lib"
    }
    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"
    
    filter "configurations:Release"
        defines{"NDEBUG"}
        optimize "On" 



project "Test"
        location"Test"
        kind "ConsoleApp"
        language "C++"
        targetdir "Test/bin/"
        objdir "bin/intermediate"
        
        dependson{"imGui","glfw","Opengl1"}
        files
        {
            "%{prj.location}/Header Files/**.h",
            "%{prj.location}/Source Files/**.cpp",
        }
    
            includedirs
        {
            "glfw/include",
            "Opengl1/vendor",
            "Opengl1/Header Files",
            "Opengl1/vendor",
            "Opengl1/vendor/imGui",
            "Opengl1/vendor/assimp/include",
            "ECS/HeaderFiles"
        }
    
        libdirs
        {
            "glfw/src/Debug",
            "Opengl1/vendor/imGui/windows/Debug",
            "Opengl1/vendor/assimp/lib/Debug",
            "Opengl1/bin"
        }
    
        links
        {
            "glfw3.lib",
            "opengl32.lib",
            "imGui.lib",
            "assimp-vc142-mtd.lib",
            "Opengl1.lib"
        }
        filter "configurations:Debug"
            defines {"DEBUG"}
            symbols "On"
        
        filter "configurations:Release"
            defines{"NDEBUG"}
            optimize "On" 