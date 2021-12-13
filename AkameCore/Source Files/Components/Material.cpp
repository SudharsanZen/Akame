#include "Components/Rendering/Material.h"
#include"Core/Log/Log.h"
#include"Rendering/Camera.h"
unsigned long long shaderCount = 0;
//set the texture uniform and it's value
Material::Material(std::string shaderName)
{
	materialID = std::make_shared<unsigned long long>();
	*materialID = shaderCount++;
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
		AssetManager::notUsingTex(itr->second.location);
		itr->second = td;
	}

}
void Material::isEmissive(bool em)
{
	emissive = em;
}


void Material::setUniformsOnce(std::shared_ptr<Shader> shader, glm::vec3& viewPose)
{
	shader->setUniformVec3("viewPos", viewPose);

	for (auto& t : *uniformToTexDetails)
	{
		a.GetTexture(t.second.assetIndex)->use(t.second.texUnit);
		shader->setUniformInteger(t.first, t.second.texUnit);
	}

	for (auto& t : *uniformTofloat)
	{
		shader->setUniformFloat(t.first, t.second);
	}

	transformLocation = shader->getUniformLocation("transform");
	transformIndexLocation = shader->getUniformLocation("t_index");


}

void Material::setUniformOnceSkeletalMesh(std::shared_ptr<Shader> shader, Camera cam)
{
	//set all texture details
	for (auto& t : *uniformToTexDetails)
	{
		a.GetTexture(t.second.assetIndex)->use(t.second.texUnit);
		shader->setUniformInteger(t.first, t.second.texUnit);
	}
	//set all float uniform variables and their values
	for (auto& t : *uniformTofloat)
	{
		shader->setUniformFloat(t.first, t.second);
	}
}

void Material::setUniformEveryObject(int index, std::shared_ptr<Shader> shader)
{

	
	//shader->setUniformMat4fv(transformLocation, 1, &(t.transformMat[0].x));
	shader->setUniformInteger(transformIndexLocation, index);
}

void Material::reset()
{
	*refCount -= 1;
	//std::cout << "\n usecount:" << *refCount;
	if (*refCount == 0)
	{

		if (uniformToTexDetails)
		{
			for (auto& t : *uniformToTexDetails)
			{
				AssetManager::notUsingTex(t.second.location);
			}
			uniformToTexDetails.reset();
		}
	}
}

Material::~Material()
{
	reset();
}

