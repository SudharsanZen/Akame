project "ECS"
    location"%{wks.location}/ECS"
    kind "StaticLib"
    language "C++"
    objdir "%{wks.location}/bin/Intermediate/%{cfg.buildcfg}/%{prj.name}"
    targetdir "%{prj.location}/%{cfg.system}/%{cfg.buildcfg}"
    files
    {
        "%{prj.location}/HeaderFiles/**.h",
        "%{prj.location}/SourceFiles/**.cpp",
    }

    includedirs
    {
        "%{prj.location}/HeaderFiles",
    }

    filter "configurations:Debug"
    defines {"DEBUG"}
    symbols "On"

    filter "configurations:Release"
    defines{"NDEBUG"}
    optimize "On" 