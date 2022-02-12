call "cmake" AkameCore\vendor\glfw -B AkameCore\vendor\glfw\builds -G "Visual Studio 16 2019" -D BUILD_SHARED_LIBS=1
call "cmake" AkameCore\vendor\assimp -B AkameCore\vendor\assimp -G "Visual Studio 16 2019"
call "cmake" AkameCore\vendor\pugixml -B AkameCore\vendor\pugixml -G "Visual Studio 16 2019"
echo 13 | AkameCore\vendor\PhysX\physx\generate_projects.bat
call "premake5.exe" vs2019
PAUSE