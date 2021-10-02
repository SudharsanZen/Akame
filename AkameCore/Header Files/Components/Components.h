#pragma once
#include"ECS.h"
#include<memory>

class Components
{
	
protected:
	friend class Scene;
	friend class Behaviour;
	template<typename T>
	friend class ComponentArray;
	std::weak_ptr<ECS> ecs;
	Entity eid;
	e_index componentIndex;
	e_index componentBitPose;
};

