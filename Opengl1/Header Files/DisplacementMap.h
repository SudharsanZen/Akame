#pragma once
#include"glm/glm/glm.hpp"
#include"Texture.h"
#include"Shader.h"
#include"engine_constants.h"
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

	GLfloat ambientIntensity;

	
public:
	std::shared_ptr<Shader> shader;

	void isEmissive(bool em)
	{
		emissive = em;
	}
	DisplacementMap()
	{
		emissive = false;
		diffColor = glm::vec3(1, 1, 1);
		specularColor = glm::vec3(1, 1, 1);
		ambientIntensity = 0.2f;
		normalMapOn = false;
		
		shader = std::make_shared<Shader>(ASSETS_ROOT_DIR+std::string("Shaders/displacement.vert"),ASSETS_ROOT_DIR+std::string("Shaders/displacement.frag"));
		shader->compileShader();
	}
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
		GLint a = 0;

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

