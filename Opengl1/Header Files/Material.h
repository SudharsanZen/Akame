#pragma once
#include"GlmMath.h"
#include"Texture.h"
#include"Shader.h"
#include"Assets/AssetManager.h"
#include<unordered_map>
class Material
{
private:
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
public:
	Material(std::string shaderName="DEFAULT")
	{
		SHADER_NAME = shaderName;
		refCount = std::make_shared<int>();
		*refCount = 1;
		uniformToTexDetails = std::make_shared<std::unordered_map<std::string, texDetails>>();
		uniformTofloat = std::make_shared<std::unordered_map<std::string, float>>();
		emissive = false;
		diffColor = glm::vec3(1, 1, 1);
		specularColor = glm::vec3(1, 1, 1);
		ambientIntensity = 0.2f;
	}
	//set float uniforms and it's values
	void setValue(std::string varName,float value)
	{
		auto itr = uniformTofloat->find(varName);
		if (itr == uniformTofloat->end())
		{
			(*uniformTofloat)[varName] = value;
		}
		else
		{
			//reduce reference count since the existing texture is replace by a new texture location
			itr->second = value;
		}
	}
	//set the texture uniform and it's value
	void setTexture2D(std::string uniformName,std::string location)
	{
		auto itr = uniformToTexDetails->find(uniformName);
		
		texDetails td;
		td.location = location;
		long long index = a.createTexture(location);
		td.assetIndex = index;
		td.texUnit = nextTexUnit++;

		if (itr == uniformToTexDetails->end())
		{
			(*uniformToTexDetails)[uniformName] = td;
		}
		else
		{
			//reduce reference count since the existing texture is replace by a new texture location
			a.notUsingTex(itr->second.location);
			itr->second =td;
		}

	}
	void isEmissive(bool em)
	{
		emissive = em;
	}
	
	Material(const Material& mat)
	{
		*mat.refCount += 1;
		this->refCount = mat.refCount;
		this->emissive = mat.emissive;
		this->diffColor = mat.diffColor;
		this->uniformToTexDetails = mat.uniformToTexDetails;
		this->uniformTofloat = mat.uniformTofloat;
		this->ambientIntensity = mat.ambientIntensity;
		this->SHADER_NAME = mat.SHADER_NAME;
	}
	void setUniforms(std::shared_ptr<Shader> shader,glm::vec3& lightPose, glm::vec3& viewPose)
	{

		for (auto& t : *uniformToTexDetails)
		{
			a.GetTexture(t.second.assetIndex)->use(t.second.texUnit);
			shader->setUniformInteger(t.first,t.second.texUnit);
		}

		for (auto& t : *uniformTofloat)
		{
			shader->setUniformFloat(t.first,t.second);
		}

		shader->setUniformVec3("lightPos", lightPose);
		shader->setUniformVec3("viewPos", viewPose);




		shader->setUniformInteger("emissive", (int)emissive);
		shader->setUniformVec3("lightColor", glm::vec3(1, 1, 1));
		shader->setUniformVec3("objectColor", glm::vec3(1, 1, 1));
	}
	void use(Transform &t,glm::vec3 &lightPose,glm::vec3 &viewPose,std::shared_ptr<Shader> shader)
	{
		
		setUniforms(shader,lightPose,viewPose);
		glm::mat4 trans =t.transformMatrix();
		shader->setUniformVec3("viewPos", viewPose);
		shader->setUniformMat4fv("transform", 1, glm::value_ptr(trans));
	}
	
	

	void reset() 
	{
		
		for (auto& t : *uniformToTexDetails)
		{
			a.notUsingTex(t.second.location);
		}

	}

	~Material()
	{
		*refCount -= 1;
		if (*refCount < 1)
		{
			reset();
		}
	}
};

