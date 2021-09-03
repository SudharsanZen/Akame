#include"Assets/ShaderManager.h"
#include<pugixml.hpp>
#include"Core/Log/Log.h"
#include"Assets/AssetManager.h"
#include<iostream>
#include<sstream>
#include"Rendering\System\ShaderRenderPipeline.h"
std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderManager::shaderList;
std::map<std::string, std::vector<std::pair<int, std::string>>> ShaderManager::shaderQueues;
std::map<std::string, std::shared_ptr<ShaderRenderPipeline>> ShaderManager::shaderRenderPipeline;

bool queuePairCompare(std::pair<int,std::string> a,std::pair<int,std::string> b)
{
	return a.first < b.first;
}
bool ShaderManager::checkForPipeline(std::string shaderName)
{
	if (shaderRenderPipeline.find(shaderName) != shaderRenderPipeline.end())
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

		auto itr = shaderList.find(shaderName);
		if (itr == shaderList.end())
		{
			ENGINE_CORE_INFO("ShaderName:"+shaderName);
			ENGINE_CORE_INFO("vert:path="+vertPath);
			ENGINE_CORE_INFO("frag:path="+fragPath);
			shaderList[shaderName] = std::make_shared<Shader>(vertPath,fragPath);
			if (queueName != "NULL")
			{
				auto fitr = shaderQueues.find(queueName);
				if (fitr == shaderQueues.end())
				{
					std::vector<std::pair<int, std::string>> shaderNameList;
					
					shaderQueues[queueName] = shaderNameList;
				}
				shaderQueues[queueName].push_back({ queueNumber,shaderName });
			}
		}
		else
		{
			itr->second.reset();
			itr->second = std::make_shared<Shader>(vertPath,fragPath);
		}
	}

	ENGINE_CORE_INFO("ShaderQueue");
	for (auto& q : shaderQueues)
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
