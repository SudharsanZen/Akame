#include "Core/Editor/EntityDescriptionSystem.h"
#include"Core/Log/Log.h"
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
