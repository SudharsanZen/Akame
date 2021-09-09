#pragma once
#include"Math/GlmMath.h"
#include"Rendering/Texture.h"
#include"Rendering/Shader.h"

class DisplacementMap
{
private:
	glm::vec3 diffColor;
	std::shared_ptr<Texture> diffuse;

	bool emissive = false;
	glm::vec3 specularColor;
	std::shared_ptr<Texture> roughness;

	bool normalMapOn;
	std::shared_ptr<Texture> displacementMap;

	float ambientIntensity;

	
public:
	std::shared_ptr<Shader> shader;

	void isEmissive(bool em)
	{
		emissive = em;
	}
	DisplacementMap();
	DisplacementMap(const DisplacementMap& mat)
	{
		this->emissive = mat.emissive;
		this->diffColor = mat.diffColor;
		this->diffuse = mat.diffuse;
		this->roughness = mat.roughness;
		this->displacementMap= mat.displacementMap;
		this->shader = mat.shader;
		ambientIntensity = mat.ambientIntensity;
	}
	void use(Transform& t, glm::vec3& lightPose, glm::vec3& viewPose)
	{
		glm::mat4 trans = t.transformMatrix();
		shader->useShaderProgram();
		if (diffuse)
			diffuse->use(0);
		if (roughness)
			roughness->use(1);
		if (displacementMap)
			displacementMap->use(2);

		shader->setUniformVec3("lightPos", lightPose);
		shader->setUniformVec3("viewPos", viewPose);
		shader->setUniformInteger("material.diffuseMap", 0);
		shader->setUniformInteger("material.specularMap", 1);
		shader->setUniformInteger("material.dispMap", 2);
		int a = 0;

		if (emissive)
			a = 2;
		shader->setUniformInteger("emissive", a);
		shader->setUniformVec3("lightColor", glm::vec3(1, 1, 1));
		shader->setUniformVec3("objectColor", glm::vec3(1, 1, 1));
		shader->setUniformMat4fv("transform", 1, glm::value_ptr(trans));
	}

	void setDiffuseMap(std::string fileLocation) { diffuse = std::make_shared<Texture>(fileLocation); }


	void setSpecularMap(std::string fileLocation) { roughness = std::make_shared<Texture>(fileLocation); }


	void setDisplacementMap(std::string fileLocation) { displacementMap = std::make_shared<Texture>(fileLocation); }

	void reset() {}


};

