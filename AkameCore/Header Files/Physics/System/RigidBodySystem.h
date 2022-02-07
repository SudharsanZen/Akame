#pragma once

#include"Core/AkameCore.h"
class Scene;
class ECS;
class System;

namespace physics
{
	class Physics;

	class AKAME_API RigidBodySystem :public System
	{
		std::shared_ptr<Physics> mPhysics;
		std::weak_ptr<ECS> ecs;
		friend class ::Scene;
		float accumulator = 0.0f;
		float mixAmt = 0.0f;
	public:
		RigidBodySystem();
		
		void Run(float deltaTime);
	
		void setVisualToPXTransform();
		

		void setPxToVisualTransform();
		

		void OnAddEntity(Entity entity) override;
	

	};
}
