#pragma once

#include"components/EntityDescriptor.h"

#include<map>

class EntityDescriptionSystem:public System
{
private:
	friend class Scene;
	bool m_needs_update;
	void updateList();
	std::weak_ptr<ECS> ecs;
public:
	AKAME_API void updateMap();
	std::map<std::string, std::vector<Entity>> tagMap;

	AKAME_API std::vector<Entity> GetEntitiesWithTag(std::string tag);
	
	AKAME_API void SetEntityTag(Entity eid, std::string tag);

	AKAME_API void OnAddEntity(Entity eid)override;

	AKAME_API void OnDestroyEntity(Entity eid) override;
};

