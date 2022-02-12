#pragma once

#include<memory>
#include"Core/AkameCore.h"
#include"System.h"
class Components
{
	
protected:
	friend class Scene;
	friend class Behaviour;
	template<typename T>
	friend class ComponentArray;
	std::weak_ptr<ECS> ecs;
	Entity eid;
	e_index componentIndex=-1;
	e_index componentBitPose=-1;
	
};

