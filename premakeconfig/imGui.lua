project "imGui"
    location"%{wks.location}/AkameCore/vendor/imGui"
    kind "StaticLib"
    language "C++"
    objdir "%{wks.location}/bin/Intermediate/%{cfg.buildcfg}/%{prj.name}"
    targetdir "%{prj.location}/%{cfg.system}/%{cfg.buildcfg}"
    dependson{"glfw"}
    files
    {
        "%{prj.location}/*.h",
        "%{prj.location}/*.c",
        "%{prj.location}/*.cpp",
        "%{prj.location}/misc/cpp/*.h",
        "%{prj.location}/misc/cpp/*.cpp",
        "%{wks.location}/AkameCore/vendor/glad-akame-/glad/glad.c",
        "%{prj.location}/backends/imgui_impl_opengl3.cpp",
        "%{prj.location}/backends/imgui_impl_opengl3.h",
        "%{prj.location}/backends/imgui_impl_glfw.h",
        "%{prj.location}/backends/imgui_impl_glfw.cpp"
    }

    includedirs
    {
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.vendor}",
        "%{prj.location}"
    }

    libdirs
    {
              
    }

    links
    {
        
    }
    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"
    
    filter "configurations:Release"
        defines{"NDEBUG"}
        optimize "On" 