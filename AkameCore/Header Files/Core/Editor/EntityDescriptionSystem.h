#pragma once
#include"ECS.h"
#include"components/EntityDescriptor.h"

#include<map>

class AKAME_API EntityDescriptionSystem:public System
{
private:
	friend class Scene;
	bool m_needs_update;
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
	void updateMap()
	{
		if (m_needs_update)
		{
			updateList();
			m_needs_update = false;
		}
	}
	std::map<std::string, std::vector<Entity>> tagMap;

	std::vector<Entity> GetEntitiesWithTag(std::string tag);
	
	void SetEntityTag(Entity eid, std::string tag)
	{
		EntityDescriptor& ed = ecs.lock()->GetComponent<EntityDescriptor>(eid);
		ed.SetTag(tag);
		m_needs_update = true;
		//updateList();
	}

	void OnAddEntity(Entity eid)override
	{
		m_needs_update = true;
		//updateList();
	}

	void OnDestroyEntity(Entity eid) override
	{
		m_needs_update = true;
		//updateList();
	}
};

