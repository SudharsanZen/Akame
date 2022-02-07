#pragma once
#include"ECS.h"
#include"Components/Lighting/Lights.h"
#include"Core/Log/Log.h"
#include"Rendering/System/ShadowFBO.h"
#include"Rendering/Camera.h"
#include"Rendering/System/PSSMFBO.h"
#define DIR_MAP_SIZE 2048
#define FRUSTUM_SPLIT_NUM 3
class LightSystem:public System
{
private:
	
	//struct for use in uniform buffer
	struct pointLight
	{
		glm::vec4 lightPose; 
		glm::vec4 lightColor;
		glm::vec4 ambient; 
		glm::vec4 constants;
		float intensity;
		float radius;
	};

	struct directionalLight
	{
		glm::vec3 lightDir;
		glm::vec3 lightColor;
		glm::vec3 ambient;
		float intensity;
	};
	

	//pointLight uniform buffer
	unsigned int plUBO;
	std::weak_ptr<ECS> ecs;
	std::unordered_map<LIGHT, std::vector<Entity>> lightsList;
	AKAME_API void updateLightList();
	
	static bool needsUpdate;
	friend class Scene;
	friend class Lights;
	friend class RenderingSystem;
	friend class TiledRenderer;
	friend class Editor;
public:
	static float pssmXMultiplier;
	static float pssmYMultiplier;
	float shadowDistance = 350;
	//point light vector for storing in point light uniform buffer
	std::vector<pointLight> ptVector;
	std::vector<directionalLight> drVector;
	std::vector<glm::mat4> dirLightSpace;
	float lambda = 0.238f;
	PSSMFBO dir_sMap;
	//empty point light list completely and create a new one, call when entity is added or removed
	AKAME_API void updatePointLightBuffer();
	//calculates and sets the necesary variables for shadow map calculations
	AKAME_API void BindDirectionalLightShadowMap(std::shared_ptr<Shader> shader,Camera &cam);

	//get the directional light's view projection matrix
	AKAME_API std::vector<glm::mat4> GetDirectionalLightSpaceMat();
	AKAME_API void unBindDirectionalShadowMap(unsigned int frameBuffer=0);

	//just update the information for existing lights, call for dynamic point lights
	AKAME_API void updatePointLightContents();
	AKAME_API void bindPointLightBuffer(int layoutIndex);
	AKAME_API LightSystem();
	AKAME_API void Update();
	AKAME_API void OnAddEntity(Entity entity) override;

	AKAME_API void AfterDestroyEntity() override;

};
