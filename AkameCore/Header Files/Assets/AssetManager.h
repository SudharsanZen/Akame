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
	AKAME_API static std::string assetRootPath;
	AKAME_API AssetManager();
	//call this once to initialize assets manager
	AKAME_API static void init();
	AKAME_API static long long createTexture(std::string location);
	AKAME_API static std::shared_ptr<Texture> GetTexture(long long index);
	AKAME_API static void notUsingTex(std::string location);

	AKAME_API std::shared_ptr<Shader> getShader(std::string shaderName);
	
	AKAME_API static bool isInitialized();
	AKAME_API static void setAssetRoot(std::string root);
	AKAME_API static std::string getAssetRoot();
	AKAME_API static void reloadAllShaders();

	AKAME_API static void reset();

	
};

