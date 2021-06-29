#pragma once
#include"Assets/AssetArray.h"
#include<map>
#include"Texture.h"
#include"Shader.h"
class AssetManager
{
private:
	static AssetArray<Texture> _tex;
	static std::map<std::string, long long> texLocIndexList;//maps image location to texture class index or id
	bool textureAlreadyExists(std::string location);
public:
	AssetManager();
	long long createTexture(std::string location);
	std::shared_ptr<Texture>GetTexture(long long index);
	void notUsingTex(std::string location);
};

