
--[[GLFW--]]
externalproject "glfw"
	 location "%{wks.location}/glfw/src"
	 kind "StaticLib"
project "ALL_BUILD_DEP"
	 location "%{wks.location}/Opengl1/vendor"
	 kind "Utility"
	 dependson{"ALL_BUILD_PHYSX","ALL_BUILD_ASSIMP","glfw","imGui"}
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
			location "%{wks.location}/Opengl1/vendor/PhysX/physx/compiler/vc16win64/sdk_source_bin"
			kind "StaticLib"

	end

	project "ALL_BUILD_PHYSX"
			location "%{wks.location}/Opengl1/vendor/PhysX/physx/compiler/vc16win64/sdk_source_bin"
			kind "Utility"
			dependson(physxProjs)

group ""
	
group "Dependencies/Assimp"

	externalproject ("assimp")
				location "%{wks.location}/Opengl1/vendor/assimp/code"
				kind "StaticLib"
	externalproject ("zlibstatic")
				location "%{wks.location}/Opengl1/vendor/assimp/contrib/zlib"
				kind "StaticLib"
	project "ALL_BUILD_ASSIMP"
				location "%{wks.location}/Opengl1/vendor/PhysX/physx/compiler/vc16win64/sdk_source_bin"
				kind "Utility"
				dependson{"assimp","zlibstatic"}
group ""

