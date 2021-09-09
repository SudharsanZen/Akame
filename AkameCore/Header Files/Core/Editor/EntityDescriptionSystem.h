#pragma once
#include"ECS.h"
#include"components/EntityDescriptor.h"

#include<map>

class EntityDescriptionSystem:public System
{
private:
	friend class Scene;
	void updateList()
	{
		tagMap.clear();
		std::shared_ptr<ECS> e = ecs.lock();
		for (auto ent : entities)
		{
			EntityDescriptor& ed = e->GetComponent<EntityDescriptor>(ent);
			tagMap[ed.tag].push_back(ed.eid);
		}
	}
	std::weak_ptr<ECS> ecs;
public:

	std::map<std::string, std::vector<EntityID>> tagMap;

	std::vector<EntityID> GetEntitiesWithTag(std::string tag);
	
	void SetEntityTag(EntityID eid, std::string tag)
	{
		EntityDescriptor& ed = ecs.lock()->GetComponent<EntityDescriptor>(eid);
		ed.SetTag(tag);
		updateList();
	}

	void OnAddEntity(Entity eid)override
	{
		updateList();
	}

	void OnDestroyEntity(Entity eid) override
	{
		updateList();
	}
};

