#pragma once
#include "ECS.h"
#include "Physics\Physics.h"

namespace physics
{
	class PhysicsSystem : public System
	{
		std::shared_ptr<Physics> mPhysics;
		std::weak_ptr<ECS> ecs;
	public:
		PhysicsSystem()
		{
			mPhysics = std::make_shared<Physics>();
		}

	};
}

