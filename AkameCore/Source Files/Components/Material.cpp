#include "Components/Rendering/Material.h"
#include"Core/Log/Log.h"

//set the texture uniform and it's value
Material::Material(std::string shaderName)
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

void Material::setValue(std::string varName, float value)
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

void Material::setTexture2D(std::string uniformName, std::string location)
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
		itr->second = td;
	}

}
void Material::isEmissive(bool em)
{
	emissive = em;
}


void Material::setUniforms(std::shared_ptr<Shader> shader, glm::vec3& lightPose, glm::vec3& viewPose)
{

	for (auto& t : *uniformToTexDetails)
	{
		a.GetTexture(t.second.assetIndex)->use(t.second.texUnit);
		shader->setUniformInteger(t.first, t.second.texUnit);
	}

	for (auto& t : *uniformTofloat)
	{
		shader->setUniformFloat(t.first, t.second);
	}

	shader->setUniformVec3("lightPos", lightPose);
	shader->setUniformVec3("viewPos", viewPose);




	shader->setUniformInteger("emissive", (int)emissive);
	shader->setUniformVec3("lightColor", glm::vec3(1, 1, 1));
	shader->setUniformVec3("objectColor", glm::vec3(1, 1, 1));
}

void Material::use(Transform& t, glm::vec3& lightPose, glm::vec3& viewPose, std::shared_ptr<Shader> shader)
{

	setUniforms(shader, lightPose, viewPose);
	glm::mat4 trans = t.transformMatrix();
	shader->setUniformVec3("viewPos", viewPose);
	shader->setUniformMat4fv("transform", 1, glm::value_ptr(trans));
}

void Material::reset()
{

	refCount.reset();

}

Material::~Material()
{
	
	if (refCount.use_count()<=1)
	{
		for (auto& t : *uniformToTexDetails)
		{
			a.notUsingTex(t.second.location);
		}
	}
}

