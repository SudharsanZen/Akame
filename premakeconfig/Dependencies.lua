
--[[GLFW--]]
externalproject "glfw"
	 location "%{IncludeDir.vendor}/glfw/src"
	 kind "StaticLib"
project "ALL_BUILD_DEP"
	 location "%{IncludeDir.vendor}"
	 kind "Utility"
	 dependson{"ALL_BUILD_PHYSX","ALL_BUILD_ASSIMP","glfw","imGui","pugixml"}
--[[Physx--]]
group "Dependencies/Physx"
	physxProjs={

		"FastXml","LowLevel","LowLevelAABB","LowLevelDynamics","PhysX","PhysXCharacterKinematic",
		"PhysXCommon","PhysXCooking","PhysXExtensions","PhysXFoundation","PhysXPvdSDK","PhysXTask",
		"PhysXVehicle","SceneQuery","SimulationController"
	}

	for k,v in ipairs(physxProjs)
	do

		externalproject (v)
			location "%{IncludeDir.vendor}/PhysX/physx/compiler/vc16win64/sdk_source_bin"
			kind "StaticLib"
			configurations {"Release" }

			filter "configurations:Release"
			defines { "NDEBUG" }
			optimize "On"

			filter "configurations:Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
			buildoptions "/MDd"


	end

	project "ALL_BUILD_PHYSX"
			location "%{IncludeDir.vendor}/PhysX/physx/compiler/vc16win64/sdk_source_bin"
			kind "Utility"
			dependson(physxProjs)

group ""

group "Dependencies/Assimp"

	externalproject ("assimp")
				location "%{IncludeDir.vendor}/assimp/code"
				kind "StaticLib"
	externalproject ("zlibstatic")
				location "%{IncludeDir.vendor}/assimp/contrib/zlib"
				kind "StaticLib"
	project "ALL_BUILD_ASSIMP"
				location "%{IncludeDir.vendor}/PhysX/physx/compiler/vc16win64/sdk_source_bin"
				kind "Utility"
				dependson{"assimp","zlibstatic"}
group ""

group "Dependencies/pugixml"
	project "pugixml"
			location"%{IncludeDir.vendor}/pugixml/src"
			kind "StaticLib"
			language "C++"
			targetdir "%{prj.location}/bin/%{cfg.buildcfg}"
			objdir "%{wks.location}/bin/Intermediate/%{cfg.buildcfg}/%{prj.name}"

			files
			{
				"%{prj.location}/**.hpp",
				"%{prj.location}/**.cpp",
			}


			filter "configurations:Debug"
            defines {"DEBUG"}
            symbols "On"

			filter "configurations:Release"
            defines{"NDEBUG"}
            optimize "On"

group""
