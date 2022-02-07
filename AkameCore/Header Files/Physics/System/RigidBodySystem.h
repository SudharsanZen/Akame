#pragma once

#include"Core/AkameCore.h"
class Scene;
class ECS;
class System;

namespace physics
{
	class Physics;

	class RigidBodySystem :public System
	{
		std::shared_ptr<Physics> mPhysics;
		std::weak_ptr<ECS> ecs;
		friend class ::Scene;
		float accumulator = 0.0f;
		float mixAmt = 0.0f;
	public:
		AKAME_API RigidBodySystem();
		
		AKAME_API void Run(float deltaTime);
	
		AKAME_API void setVisualToPXTransform();
		

		AKAME_API void setPxToVisualTransform();
		

		AKAME_API void OnAddEntity(Entity entity) override;
	

	};
}
