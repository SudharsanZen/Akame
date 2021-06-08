call "cmake" glfw -B glfw -G "Visual Studio 16 2019"
call "cmake" Opengl1\vendor\assimp -B Opengl1\vendor\assimp -G "Visual Studio 16 2019"
echo 13 | Opengl1\vendor\PhysX\physx\generate_projects.bat
call "premake5.exe" vs2019