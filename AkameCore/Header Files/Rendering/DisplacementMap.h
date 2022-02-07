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

	AKAME_API void isEmissive(bool em);
	AKAME_API DisplacementMap();
	AKAME_API DisplacementMap(const DisplacementMap& mat);
	AKAME_API void use(Transform& t, glm::vec3& lightPose, glm::vec3& viewPose);
	AKAME_API void setDiffuseMap(std::string fileLocation);
	AKAME_API void setSpecularMap(std::string fileLocation);
	AKAME_API void setDisplacementMap(std::string fileLocation);
	AKAME_API void reset();


};

