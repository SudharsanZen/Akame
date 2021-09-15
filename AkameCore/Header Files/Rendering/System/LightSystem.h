#pragma once
#include"ECS.h"
#include"Components/Lighting/Lights.h"
#include"Core/Log/Log.h"
#include"Rendering/System/ShadowFBO.h"
#include"Rendering/Camera.h"
#define DIR_MAP_SIZE 2048
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
	void updateLightList()
	{
		lightsList.clear();
		for (auto const& ent : entities)
		{
			Lights& l = ecs.lock()->GetComponent<Lights>(ent);
			lightsList[l.getType()].push_back(ent);
		}
	}
	glm::mat4 dirLightSpace = glm::mat4(0);
	
	friend class Scene;
	friend class Lights;
	friend class RenderingSystem;
	friend class TiledRenderer;
	friend class Editor;
public:
	//point light vector for storing in point light uniform buffer
	std::vector<pointLight> ptVector;
	std::vector<directionalLight> drVector;

	ShadowFBO dir_sMap;
	//empty point light list completely and create a new one, call when entity is added or removed
	void updatePointLightBuffer();
	//calculates and sets the necesary variables for shadow map calculations
	void BindDirectionalLightShadowMap(std::shared_ptr<Shader> shader,Camera &cam);
	
	//get the directional light's view projection matrix
	glm::mat4 GetDirectionalLightSpaceMat()
	{
		return dirLightSpace;
	}
	void unBindDirectionalShadowMap();

	//just update the information for existing lights, call for dynamic point lights
	void updatePointLightContents();
	void bindPointLightBuffer(int layoutIndex);
	LightSystem();
	
	void OnAddEntity(Entity entity) override
	{
		
		updateLightList();
		updatePointLightBuffer();

		

	}

	void AfterDestroyEntity() override
	{
		updateLightList();
		updatePointLightBuffer();
	}

};

