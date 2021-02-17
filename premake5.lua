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
    targetdir "bin/"
    objdir "bin/intermediate"
    dependson{"imGui","glfw"}
    files
    {
        "%{prj.location}/Header Files/**.h",
        "%{prj.location}/Source Files/**.cpp",
    }

    includedirs
    {
        "glfw/include",
        "Opengl1/vendor",
        "%{prj.location}/Header Files",
        "%{prj.location}/vendor",
        "%{prj.location}/vendor/imGui"
    }

    libdirs
    {
        "glfw/src/Debug",
        "%{prj.location}/vendor/imGui/windows/Debug",
    }

    links
    {
        "glfw3.lib",
        "opengl32.lib",
        "imGui.lib"
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