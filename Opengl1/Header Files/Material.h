#pragma once
#include"glm/glm/glm.hpp"
#include"Texture.h"
#include"Shader.h"
class Material
{
private:
	glm::vec3 diffColor;
	Texture diffuse;

	glm::vec3 specularColor;
	Texture roughness;

	bool normalMapOn;
	Texture normalMap;

	GLfloat ambientIntensity;

	Shader shader;
public:
	
	Material(Texture &dif):diffuse(dif)
	{
		diffColor = glm::vec3(1,1,1);
		specularColor = glm::vec3(1,1,1);
		ambientIntensity = 0.2f;
		normalMapOn = false;
		//shader.compileShader();
	}

	Material()
	{
		diffColor = glm::vec3(1, 1, 1);
		specularColor = glm::vec3(1, 1, 1);
		ambientIntensity = 0.2f;
		normalMapOn = false;
		//shader.compileShader();
	}

	void use(Transform &t,glm::vec3 &lightPose,glm::vec3 &viewPose)
	{
		glm::mat4 trans =t.transformMatrix();
		shader.useShaderProgram();
		
		diffuse.use(0);
		roughness.use(1);
		normalMap.use(2);

		shader.setUniformVec3("lightPos", lightPose);
		shader.setUniformVec3("viewPos", viewPose);
		shader.setUniformInteger("material.diffuseMap",0);
		shader.setUniformInteger("material.specularMap",1);
		shader.setUniformInteger("material.normalMap",2);
		shader.setUniformVec3("lightColor", glm::vec3(1, 1, 1));
		shader.setUniformVec3("objectColor", glm::vec3(1, 1, 1));
		shader.setUniformMat4fv("transform", 1, glm::value_ptr(trans));
	}
	
	void setDiffuseMap(std::string fileLocation,GLuint imageFormat)		{		diffuse		= Texture(fileLocation,imageFormat);		}
	void setSpecularMap(std::string fileLocation, GLuint imageFormat)	{		roughness	= Texture(fileLocation, imageFormat);		}
	void setNormalMap(std::string fileLocation, GLuint imageFormat)		{		normalMap	= Texture(fileLocation, imageFormat);	}



};

