#pragma once
#include"Rendering/Shader.h"
#include<unordered_map>
#include<map>
#include<queue>
class ShaderRenderPipeline;
class ShaderManager
{
	static std::map<std::string, std::vector<std::pair<int, std::string>>> shaderQueues;
	static std::unordered_map<std::string,std::shared_ptr<Shader>> shaderList;
	static std::map<std::string, std::shared_ptr<ShaderRenderPipeline>> shaderRenderPipeline;
	static bool checkForPipeline(std::string shaderName);
	friend class RenderingSystem;

public:

	ShaderManager();
	static void loadAllShaders();
	static std::shared_ptr<Shader> GetShader(std::string shaderName);
	static void releaseAllShaders() 
	{
		shaderList.clear();
	}
	template<typename T>
	static void AttachShaderPipeline(std::string SHADERNAME) 
	{
		
		shaderRenderPipeline[SHADERNAME] = std::make_shared<T>();

	}
	//void loadShader(std::string shaderName);
};
