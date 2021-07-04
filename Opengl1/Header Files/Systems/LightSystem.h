#pragma once
#include"ECS.h"
#include"Lights.h"
class LightSystem:public System
{
private:
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
public:

	void OnAddEntity(Entity entity) override
	{
		
		updateLightList();
	}

	void OnDestroyEntity(Entity entity) override
	{
		updateLightList();
	}

};

