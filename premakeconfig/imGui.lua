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
        "%{wks.location}/AkameCore/vendor/glad/glad.c",
        "%{prj.location}/backends/imgui_impl_opengl3.cpp",
        "%{prj.location}/backends/imgui_impl_opengl3.h",
        "%{prj.location}/backends/imgui_impl_glfw.h",
        "%{prj.location}/backends/imgui_impl_glfw.cpp"
    }

    includedirs
    {
        "%{IncludeDir.GLFW}",
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