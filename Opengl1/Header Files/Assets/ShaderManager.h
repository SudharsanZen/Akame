#pragma once
#include"Shader.h"
#include<unordered_map>
class ShaderManager
{
	static std::unordered_map<std::string,std::shared_ptr<Shader>> shaderList;
	friend class RenderingSystem;
public:

	ShaderManager();
	static void loadAllShaders();
	static std::shared_ptr<Shader> GetShader(std::string shaderName);
	static void releaseAllShaders() 
	{
		shaderList.clear();
	}
	//void loadShader(std::string shaderName);
};
