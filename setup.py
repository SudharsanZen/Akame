import os
import subprocess as sp
cmake=sp.Popen(["cmake","../glfw","-DCMAKE_TOOLCHAIN_FILE=glfw/CMake/x86_64-w64-mingw32.cmake ."])
os.system("premake5.exe vs2019")
