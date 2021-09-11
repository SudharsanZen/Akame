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
	Material(std::string shaderName = "DEFAULT");
	//set float uniforms and it's values
	void setValue(std::string varName, float value);
	//set the texture uniform and it's value
	void setTexture2D(std::string uniformName, std::string location);
	void isEmissive(bool em);
	
	

	void setUniforms(std::shared_ptr<Shader> shader, glm::vec3& lightPose, glm::vec3& viewPose);
	void use(Transform& t, glm::vec3& lightPose, glm::vec3& viewPose, std::shared_ptr<Shader> shader);
	
	

	void reset();

	~Material();
};

