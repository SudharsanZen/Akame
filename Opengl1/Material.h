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
	Texture normalmap;

	GLfloat ambientIntensity;

	Shader shader;
public:

	Material()
	{
		diffColor = glm::vec3(1,1,1);
		specularColor = glm::vec3(1,1,1);
		ambientIntensity = 0.2f;
		normalMapOn = false;
		shader.compileShader();
	}
	




};

