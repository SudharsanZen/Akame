#include"Assets/ShaderManager.h"
#include<pugixml.hpp>
#include"Log/Log.h"
#include"Assets/AssetManager.h"
#include<iostream>
std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderManager::shaderList;
ShaderManager::ShaderManager()
{
	
}

void ShaderManager::loadAllShaders()
{
	ENGINE_CORE_INFO("Loading Shaders!");
	pugi::xml_document doc;
	std::string filepath=AssetManager::getAssetRoot();
	filepath += "Shaders/ShaderConf.xml";
	pugi::xml_parse_result result = doc.load_file(filepath.c_str());
	if (!result)
	{
		ENGINE_CORE_ERROR("Can't open ShaderConf.XML! can't load shaders!");
		return;
	}
	std::string shaderRoot = AssetManager::getAssetRoot() +std::string("Shaders/");
	for (pugi::xml_node ShaderNode : doc.children("Shader"))
	{
		std::string shaderName=ShaderNode.attribute("name").value();
		std::string vertPath =shaderRoot+ShaderNode.child("Vertex").attribute("path").value();
		std::string fragPath = shaderRoot+ShaderNode.child("Fragment").attribute("path").value();

		auto itr = shaderList.find(shaderName);
		if (itr == shaderList.end())
		{
			ENGINE_CORE_INFO("ShaderName:"+shaderName);
			ENGINE_CORE_INFO("vert:path="+vertPath);
			ENGINE_CORE_INFO("frag:path="+fragPath);
			shaderList[shaderName] = std::make_shared<Shader>(vertPath,fragPath);
		}
		else
		{
			itr->second.reset();
			itr->second = std::make_shared<Shader>(vertPath,fragPath);
		}
	}

}

std::shared_ptr<Shader> ShaderManager::GetShader(std::string shaderName)
{
	auto itr = shaderList.find(shaderName);
	if (itr != shaderList.end())
	{
		return itr->second;
	}
	else
	{
		ENGINE_CORE_ERROR("NO SUCH SHADER FOUND!");
		return shaderList["ERROR"];
	}
}
