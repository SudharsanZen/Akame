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
	friend class ECS;
	friend class ComponentArray<Material>;
	void setUniformsOnce(std::shared_ptr<Shader> shader, glm::vec3& viewPose);
	void setUniformEveryObject(int index,std::shared_ptr<Shader> shader);
public:
	Material() :Material("DEFAULT") {}
	Material(std::string shaderName);
	//set float uniforms and it's values
	void setValue(std::string varName, float value);
	//set the texture uniform and it's value
	void setTexture2D(std::string uniformName, std::string location);
	void isEmissive(bool em);
	
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
		
		return *this;
	}
	
	/*Material(const Material& mat)
	{
		*mat.refCount += 1;
		this->refCount = mat.refCount;
		this->emissive = mat.emissive;
		this->diffColor = mat.diffColor;
		this->uniformToTexDetails = mat.uniformToTexDetails;
		this->uniformTofloat = mat.uniformTofloat;
		this->ambientIntensity = mat.ambientIntensity;
		this->SHADER_NAME = mat.SHADER_NAME;
	}*/


	
	
	

	void reset();

	~Material();
};

