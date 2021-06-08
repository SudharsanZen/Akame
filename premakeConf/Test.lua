project "Test"
        location"%{wks.location}/Test"
        kind "ConsoleApp"
        language "C++"
        targetdir "%{prj.location}/bin/%{cfg.buildcfg}"
        objdir "%{wks.location}/bin/Intermediate/%{cfg.buildcfg}/%{prj.name}"
        dependson{"imGui","glfw","Opengl1"}
        debugdir "%{prj.location}/bin"
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
            LibraryDir
        }
        debugenvs
        {
            AllDebugEnvPaths
        }

        links
        {
            Liblinks
        }


        filter "configurations:Debug"
            defines {"DEBUG"}
            symbols "On"
        
        filter "configurations:Release"
            defines{"NDEBUG"}
            optimize "On" 