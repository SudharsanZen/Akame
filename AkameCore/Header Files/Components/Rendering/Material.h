#pragma once
#include"Math/GlmMath.h"
#include"Rendering/Texture.h"
#include"Rendering/Shader.h"
#include"Assets/AssetManager.h"
#include"Components/Components.h"
#include<unordered_map>

class Material:public Components
{
private:
	//remake drawlists in Rendering system
	AKAME_API static bool remake_draw_list;
	bool is_static = true;
	bool is_cullable = true;
	unsigned int transformLocation;
	unsigned int transformIndexLocation;
	std::shared_ptr<unsigned long long> materialID;
	AssetManager a;
	unsigned int nextTexUnit = 0;
	std::string SHADER_NAME;
	struct texDetails
	{
		std::string location="";
		long long assetIndex=0;
		unsigned int texUnit=0;
	};
	std::shared_ptr<int>refCount;
	//stores all the Tetxure uniforms and corresponding values to be set
	std::shared_ptr<std::unordered_map<std::string, texDetails>> uniformToTexDetails;
	std::shared_ptr<std::unordered_map<std::string, float>> uniformTofloat;


	glm::vec3 diffColor;
	bool emissive=false;
	glm::vec3 specularColor;

	float ambientIntensity;

	friend class RenderingSystem;
	friend class SkeletalMeshRenderingSystem;
	friend class ECS;
	friend class ComponentArray<Material>;
	friend class ModelExporter;
	friend class InspectorWindow;


	AKAME_API void setUniformsOnce(std::shared_ptr<Shader> shader, glm::vec3& viewPose);
	AKAME_API void setUniformOnceSkeletalMesh(std::shared_ptr<Shader> shader,Camera cam);
	AKAME_API void setUniformEveryObject(int index,std::shared_ptr<Shader> shader);
public:
	AKAME_API Material();
	AKAME_API Material(std::string shaderName);
	//set float uniforms and it's values
	AKAME_API void setValue(std::string varName, float value);
	//set the texture uniform and it's value
	AKAME_API void setTexture2D(std::string uniformName, std::string location);
	AKAME_API void isEmissive(bool em);
	
	Material& operator =(const Material& mat)
	{
		*mat.refCount += 1;
		this->refCount = mat.refCount;
		this->materialID = mat.materialID;
		this->emissive = mat.emissive;
		this->diffColor = mat.diffColor;
		this->uniformToTexDetails = mat.uniformToTexDetails;
		this->uniformTofloat = mat.uniformTofloat;
		this->ambientIntensity = mat.ambientIntensity;
		this->SHADER_NAME = mat.SHADER_NAME;
		this->is_static = mat.is_static;
		this->is_cullable = mat.is_cullable;
		remake_draw_list = true;
		return *this;
	}
	

	AKAME_API void set_static(bool is_static)
	{
		remake_draw_list = true;
		this->is_static = is_static;
	}
	AKAME_API void set_cullable(bool is_cullable)
	{
		remake_draw_list = true;
		this->is_cullable = is_cullable;
	}
	AKAME_API bool is_cullable_entity()
	{
		return is_cullable;
	}
	AKAME_API bool is_static_entity()
	{
		return is_static;
	}

	AKAME_API void reset();

	AKAME_API ~Material();
};

