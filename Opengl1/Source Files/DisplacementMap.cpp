#include"DisplacementMap.h"
#include"Assets/AssetManager.h"
 DisplacementMap::DisplacementMap()
{
	emissive = false;
	diffColor = glm::vec3(1, 1, 1);
	specularColor = glm::vec3(1, 1, 1);
	ambientIntensity = 0.2f;
	normalMapOn = false;
	shader = std::make_shared<Shader>(AssetManager::getAssetRoot() + std::string("Shaders/displacement.vert"), AssetManager::getAssetRoot() + std::string("Shaders/displacement.frag"));
	shader->compileShader();
}
