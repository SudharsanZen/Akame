#include "Assets/AssetManager.h"

#include"Texture.h"
#include"Shader.h"
#include"Log/Log.h"
#include<string>
#include<filesystem>
using namespace std::filesystem;

AssetArray<Texture> AssetManager::_tex;
std::map<std::string, long long> AssetManager::texLocIndexList;
bool AssetManager::initialized = false;
std::string AssetManager::assetRootPath = "../../Assets/";
ShaderManager AssetManager::shaderManager;
//convert string to lowercase
std::string to_lower(std::string str)
{
	for (auto& c : str)
	{
		c = tolower(c);
	}
	return str;
}
std::string mapString(std::string loc)
{
	path absolutePath = absolute(loc);
	std::string absLoc = absolutePath.generic_string();
	return to_lower(absLoc);
}
//check if the given file exists
bool fileExists(std::string location)
{
	
	path loc = location;
	return is_regular_file(loc);
}

AssetManager::AssetManager()
{
	
}

void AssetManager::init()
{
	shaderManager.loadAllShaders();
	initialized = true;
}

long long AssetManager::createTexture(std::string location)
{
	if (!fileExists(location))
	{
		location = AssetManager::getAssetRoot() + "EngineAssets/errorTex.png";
		ENGINE_CORE_ERROR("CAN'T FIND TEXTURE:"+location);
		
	}
	std::string mapStr = mapString(location);
	auto itr = texLocIndexList.find(mapStr);

	if (itr == texLocIndexList.end())
	{
		long long index = _tex.CreateAsset(location);
		texLocIndexList[mapStr] = index;
		return index;
	}
	else
	{
		
		return itr->second;
		_tex.incrementRefcount(itr->second);
	}

	
	return -1;
}

std::shared_ptr<Texture> AssetManager::GetTexture(long long index)
{
	return _tex.GetAsset(index);
	
}

void AssetManager::notUsingTex(std::string location)
{
	std::string mapStr = mapString(location);
	auto itr = texLocIndexList.find(mapStr);

	if (itr != texLocIndexList.end())
	{
		int ref=_tex.decrementRefcount(itr->second);
		if (ref <= 0)
		{
			texLocIndexList.erase(itr);
			return;
		}
	}
}

std::shared_ptr<Shader> AssetManager::getShader(std::string shaderName)
{
	return shaderManager.GetShader(shaderName);
}

