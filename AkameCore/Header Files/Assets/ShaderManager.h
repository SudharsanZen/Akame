#pragma once
#include"Rendering/Shader.h"
#include<unordered_map>
#include<map>
#include<queue>
struct MaterialParam
{
	std::string name;
	std::string type;
};
class ShaderRenderPipeline;
class ShaderManager
{
	AKAME_API static std::map<std::string, std::vector<std::pair<int, std::string>>> m_shaderQueues;
	AKAME_API static std::unordered_map<std::string,std::shared_ptr<Shader>> m_shaderList;
	AKAME_API static std::unordered_map<std::string,std::shared_ptr<std::vector<MaterialParam>>> m_shaderUIdefList;
	AKAME_API static std::map<std::string, std::shared_ptr<ShaderRenderPipeline>> m_shaderRenderPipeline;
	AKAME_API static bool checkForPipeline(std::string shaderName);
	friend class RenderingSystem;
	friend class Editor;
	friend class SkeletalMeshRenderingSystem;
public:

	ShaderManager();
	AKAME_API static void loadAllShaders();
	AKAME_API static std::shared_ptr<Shader> GetShader(std::string shaderName);
	AKAME_API static std::shared_ptr<std::vector<MaterialParam>> GetShaderUIDefinition(std::string shaderName);
	AKAME_API static void releaseAllShaders();
	template<typename T,typename ...Args>
	static void AttachShaderPipeline(std::string SHADERNAME,Args&& ...args) 
	{
		
		m_shaderRenderPipeline[SHADERNAME] = std::make_shared<T>(std::forward<Args>(args)...);

	}
	//void loadShader(std::string shaderName);
};
