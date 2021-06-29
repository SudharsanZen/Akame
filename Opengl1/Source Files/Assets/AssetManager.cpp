#include "Assets/AssetManager.h"
#include"engine_constants.h"
#include"Texture.h"
#include"Shader.h"
#include"Log/Log.h"
#include<string>
#include<filesystem>

using namespace std::filesystem;

AssetArray<Texture> AssetManager::_tex;
std::map<std::string, long long> AssetManager::texLocIndexList;

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

//check if the texture is already loaded
bool AssetManager::textureAlreadyExists(std::string location)
{
	return false;
}

AssetManager::AssetManager()
{
	
}

long long AssetManager::createTexture(std::string location)
{
	if (!fileExists(location))
	{
		ENGINE_CORE_ERROR("CAN'T FIND TEXTURE");
		location =std::string(ASSETS_ROOT_DIR)+"EngineAssets/errorTex.png";
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
