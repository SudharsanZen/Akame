#pragma once
#include"ECS.h"
#include<memory>

class Components
{
	friend class Scene;
protected:
	std::weak_ptr<ECS> ecs;
	EntityID eid;
};

