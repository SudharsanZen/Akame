#pragma once

#include"Components/Components.h"
#include"Core/AkameCore.h"
class EntityDescriptor:public Components
{
	std::string name;
	std::string tag;
	bool selected=false;

	friend class SceneHierarchyWindow;
	friend class Editor;
	friend class EntityDescriptionSystem;
public:
	

	AKAME_API EntityDescriptor();

	AKAME_API void SetTag(std::string str);

	AKAME_API void SetName(std::string str);
	AKAME_API std::string GetName();
	AKAME_API void reset();
};

