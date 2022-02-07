#include "Core/Editor/EntityDescriptionSystem.h"
#include"ECS.h"
EntityDescriptor::EntityDescriptor()
{
	tag = "defualt";
	name = "Entity";
}

void EntityDescriptor::SetTag(std::string str)
{
	tag = str;
}

void EntityDescriptor::SetName(std::string str)
{
	name = str;
}

std::string EntityDescriptor::GetName()
{
	return name;
}

void EntityDescriptor::reset()
{
	tag = "default";
	name = "Entity";
}
