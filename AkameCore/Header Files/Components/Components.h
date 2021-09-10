#pragma once
#include"ECS.h"
#include<memory>

class Components
{
	
protected:
	friend class Scene;
	friend class Behaviour;
	std::weak_ptr<ECS> ecs;
	EntityID eid;
};

