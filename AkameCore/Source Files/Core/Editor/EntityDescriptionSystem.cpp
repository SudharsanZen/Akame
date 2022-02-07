#include "Core/Editor/EntityDescriptionSystem.h"
#include"Core/Log/Log.h"
#include"ECS.h"
void EntityDescriptionSystem::updateList()
{
	tagMap.clear();
	std::shared_ptr<ECS> e = ecs.lock();
	for (auto ent : entities)
	{
		EntityDescriptor& ed = e->GetComponent<EntityDescriptor>(ent);
		tagMap[ed.tag].push_back(ed.eid);
	}
}
void EntityDescriptionSystem::updateMap()
{
	if (m_needs_update)
	{
		updateList();
		m_needs_update = false;
	}
}
std::vector<Entity> EntityDescriptionSystem::GetEntitiesWithTag(std::string tag)
{
	std::vector<Entity> eList;

	if (tagMap.find(tag) != tagMap.end())
	{
		return tagMap[tag];
	}
	else
	{
		ENGINE_CORE_WARN("no entity with tag :'"+tag+ "'was found");
	}

	return eList;
}

void EntityDescriptionSystem::SetEntityTag(Entity eid, std::string tag)
{
	EntityDescriptor& ed = ecs.lock()->GetComponent<EntityDescriptor>(eid);
	ed.SetTag(tag);
	m_needs_update = true;
	//updateList();
}

void EntityDescriptionSystem::OnAddEntity(Entity eid)
{
	m_needs_update = true;
	//updateList();
}

void EntityDescriptionSystem::OnDestroyEntity(Entity eid)
{
	m_needs_update = true;
	//updateList();
}
