#pragma once
#include"Rendering/Shader.h"
#include<unordered_map>
#include<map>
#include<queue>
class ShaderRenderPipeline;
class ShaderManager
{
	AKAME_API static std::map<std::string, std::vector<std::pair<int, std::string>>> shaderQueues;
	AKAME_API static std::unordered_map<std::string,std::shared_ptr<Shader>> shaderList;
	AKAME_API static std::map<std::string, std::shared_ptr<ShaderRenderPipeline>> shaderRenderPipeline;
	AKAME_API static bool checkForPipeline(std::string shaderName);
	friend class RenderingSystem;
	friend class Editor;
	friend class SkeletalMeshRenderingSystem;
public:

	ShaderManager();
	AKAME_API static void loadAllShaders();
	AKAME_API static std::shared_ptr<Shader> GetShader(std::string shaderName);
	AKAME_API static void releaseAllShaders();
	template<typename T,typename ...Args>
	static void AttachShaderPipeline(std::string SHADERNAME,Args&& ...args) 
	{
		
		shaderRenderPipeline[SHADERNAME] = std::make_shared<T>(std::forward<Args>(args)...);

	}
	//void loadShader(std::string shaderName);
};
