#pragma once
#include"ECS.h"
#include"Lights.h"
#include"Log/Log.h"
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
	};
	//point light vector for storing in point light uniform buffer
	std::vector<pointLight> ptVector;

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

	friend class Scene;
	friend class Lights;
	friend class RenderingSystem;
	friend class TiledRenderer;
public:
	//empty point light list completely and create a new one, call when entity is added or removed
	void updatePointLightBuffer();

	//just update the information for existing lights, call for dynamic point lights
	void updatePointLightContents();
	void bindPointLightBuffer(int layoutIndex);
	LightSystem();

	void OnAddEntity(Entity entity) override
	{
		
		updateLightList();
		updatePointLightBuffer();

	}

	void OnDestroyEntity(Entity entity) override
	{
		updateLightList();
		updatePointLightBuffer();
	}

};

