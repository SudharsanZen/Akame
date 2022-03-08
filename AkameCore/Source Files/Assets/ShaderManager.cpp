#include"Assets/ShaderManager.h"

#pragma warning(push, 0)
	#include<pugixml.hpp>
#pragma warning(pop)
#include"json/single_include/nlohmann/json.hpp"
#include"Core/Log/Log.h"
#include"Assets/AssetManager.h"
#include<iostream>
#include<sstream>
#include"Rendering\System\ShaderRenderPipeline.h"
#include<fstream>
std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderManager::m_shaderList;
std::map<std::string, std::vector<std::pair<int, std::string>>> ShaderManager::m_shaderQueues;
std::map<std::string, std::shared_ptr<ShaderRenderPipeline>> ShaderManager::m_shaderRenderPipeline;
std::unordered_map<std::string, std::shared_ptr<std::vector<MaterialParam>>> ShaderManager::m_shaderUIdefList;
bool queuePairCompare(std::pair<int,std::string> a,std::pair<int,std::string> b)
{
	return a.first < b.first;
}
bool ShaderManager::checkForPipeline(std::string shaderName)
{
	if (m_shaderRenderPipeline.find(shaderName) != m_shaderRenderPipeline.end())
		return true;
	return false;
}
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
		std::string queue=ShaderNode.attribute("queue").value();


		std::string queueName=queue;
		int queueNumber = -1;
		//extract the queue details
		std::size_t i_plus = queue.find("+");
		if (i_plus != std::string::npos)
		{
			queueName = queue.substr(0, i_plus);
			std::stringstream str(queue.substr(i_plus, queue.length()));
			str >> queueNumber;
		}
		std::string vertPath =shaderRoot+ShaderNode.child("Vertex").attribute("path").value();
		std::string fragPath = shaderRoot+ShaderNode.child("Fragment").attribute("path").value();
		std::string mui_file_path = ShaderNode.child("MaterialUI").attribute("path").value();

		std::string geoPath = ShaderNode.child("Geometry").attribute("path").value();
		
		auto itr = m_shaderList.find(shaderName);
		if (itr == m_shaderList.end())
		{
			ENGINE_CORE_INFO("ShaderName:"+shaderName);
			ENGINE_CORE_INFO("vert:path="+vertPath);
			ENGINE_CORE_INFO("frag:path="+fragPath);
			if (mui_file_path != "")
			{
				m_shaderUIdefList[shaderName]=std::make_shared<std::vector<MaterialParam>>();
			
				std::ifstream file(shaderRoot + mui_file_path);
				ENGINE_CORE_INFO("MaterialUIDefPath:"+shaderRoot + mui_file_path);
					AK_ASSERT(file.good() && "can't open file!");
					nlohmann::json j; 
					file>>j;
					auto unif_name_type = j.at("uniforms").get<std::map<std::string, std::string>>();
					for (auto& name_type : unif_name_type)
					{
						m_shaderUIdefList[shaderName]->push_back({name_type.first,name_type.second});
					}
				file.close();
				
			}
			if (geoPath != "")
			{
				geoPath = shaderRoot +geoPath;
				ENGINE_CORE_INFO("geo:path=" + geoPath);
				m_shaderList[shaderName] = std::make_shared<Shader>(vertPath, fragPath,geoPath);
			}
			else
				m_shaderList[shaderName] = std::make_shared<Shader>(vertPath,fragPath);
			if (queueName != "NULL")
			{
				auto fitr = m_shaderQueues.find(queueName);
				if (fitr == m_shaderQueues.end())
				{
					std::vector<std::pair<int, std::string>> shaderNameList;
					
					m_shaderQueues[queueName] = shaderNameList;
				}
				m_shaderQueues[queueName].push_back({ queueNumber,shaderName });
			}
		}
		else
		{
			itr->second.reset();
			itr->second = std::make_shared<Shader>(vertPath,fragPath);
		}
	}

	ENGINE_CORE_INFO("ShaderQueue");
	for (auto& q : m_shaderQueues)
	{
		auto& v = q.second;
		sort(v.begin(),v.end(),queuePairCompare);
		std::cout << q.first;
		ENGINE_CORE_INFO("Queue"+q.first);
		for (auto& p : v)
		{
			ENGINE_CORE_INFO("{0:d} : "+p.second,p.first);
		}
	}



}

std::shared_ptr<Shader> ShaderManager::GetShader(std::string shaderName)
{
	auto itr = m_shaderList.find(shaderName);
	if (itr != m_shaderList.end())
	{
		return itr->second;
	}
	else
	{
		ENGINE_CORE_ERROR("NO SUCH SHADER FOUND!");
		return m_shaderList["ERROR"];
	}
}

std::shared_ptr<std::vector<MaterialParam>> ShaderManager::GetShaderUIDefinition(std::string shaderName)
{
	if (m_shaderUIdefList.find(shaderName) != m_shaderUIdefList.end())
		return m_shaderUIdefList[shaderName];
	else
		return nullptr;
}

void ShaderManager::releaseAllShaders()
{
	m_shaderList.clear();
	m_shaderQueues.clear();
}
