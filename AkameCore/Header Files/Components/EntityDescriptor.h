#pragma once
#include"ECS.h"
#include"Components/Components.h"
#include"Core/AkameCore.h"
class AKAME_API EntityDescriptor:public Components
{
	std::string name;
	std::string tag;
	bool selected=false;

	friend class SceneHierarchyWindow;
	friend class Editor;
	friend class EntityDescriptionSystem;
public:
	

	EntityDescriptor()
	{
		tag = "defualt";
		name = "Entity";
	}

	void SetTag(std::string str)
	{
		tag = str;
	}

	void SetName(std::string str)
	{
		name = str;
	}
	std::string GetName()
	{
		return name;
	}
	void reset()
	{
		tag = "default";
		name = "Entity";
	}
};

