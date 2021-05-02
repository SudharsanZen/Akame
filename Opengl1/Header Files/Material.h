#pragma once
#include"glm/glm/glm.hpp"
#include"Texture.h"
#include"Shader.h"
class Material
{
private:
	glm::vec3 diffColor;
	std::shared_ptr<Texture> diffuse;

	glm::vec3 specularColor;
	std::shared_ptr<Texture> roughness;

	bool normalMapOn;
	std::shared_ptr<Texture> normalMap;

	GLfloat ambientIntensity;

	std::shared_ptr<Shader> shader;
public:
	
	

	Material()
	{
		diffColor = glm::vec3(1, 1, 1);
		specularColor = glm::vec3(1, 1, 1);
		ambientIntensity = 0.2f;
		normalMapOn = false;
		shader = std::make_shared<Shader>();
		shader->compileShader();
	}
	Material(const Material& mat)
	{
		this->diffColor = mat.diffColor;
		this->diffuse = mat.diffuse;
		this->roughness = mat.roughness;
		this->normalMap = mat.normalMap;
		this->shader = mat.shader;
		ambientIntensity = mat.ambientIntensity;
	}
	void use(Transform &t,glm::vec3 &lightPose,glm::vec3 &viewPose)
	{
		glm::mat4 trans =t.transformMatrix();
		shader->useShaderProgram();
		if(diffuse)
			diffuse->use(0);
		if(roughness)
			roughness->use(1);
		if(normalMap)
			normalMap->use(2);

		shader->setUniformVec3("lightPos", lightPose);
		shader->setUniformVec3("viewPos", viewPose);
		shader->setUniformInteger("material.diffuseMap",0);
		shader->setUniformInteger("material.specularMap",1);
		shader->setUniformInteger("material.normalMap",2);
		shader->setUniformVec3("lightColor", glm::vec3(1, 1, 1));
		shader->setUniformVec3("objectColor", glm::vec3(1, 1, 1));
		shader->setUniformMat4fv("transform", 1, glm::value_ptr(trans));
	}
	
	void setDiffuseMap(std::string fileLocation)		{		diffuse		= std::make_shared<Texture>(fileLocation);		}


	void setSpecularMap(std::string fileLocation)	{		roughness	= std::make_shared<Texture>(fileLocation);		}


	void setNormalMap(std::string fileLocation)		{		normalMap	= std::make_shared<Texture>(fileLocation);	    }

	void reset() {}


};

