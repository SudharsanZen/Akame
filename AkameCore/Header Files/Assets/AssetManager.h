#pragma once
#include"Assets/AssetArray.h"
#include<map>
#include"Rendering/Texture.h"
#include"Rendering/Shader.h"
#include"Assets/ShaderManager.h"

class AssetManager
{
private:
	
	static bool initialized;
	static AssetArray<Texture> _tex;
	static std::map<std::string, long long> texLocIndexList;//maps image location to texture class index or id
	static ShaderManager shaderManager;//Manages shaders in the ASSETS_ROOT_DIR+"Shaders/" directory
	
public:
	static std::string assetRootPath;
	AssetManager();
	//call this once to initialize assets manager
	static void init();
	static long long createTexture(std::string location);
	std::shared_ptr<Texture>GetTexture(long long index);
	static void notUsingTex(std::string location);

	std::shared_ptr<Shader> getShader(std::string shaderName);
	
	static bool isInitialized() { return initialized; }
	static void setAssetRoot(std::string root) { assetRootPath = root; }
	static std::string getAssetRoot() { return assetRootPath; }
	static void reloadAllShaders() { shaderManager.loadAllShaders(); }

	static void reset() 
	{

	}

	
};

