#pragma once


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

	public:
		RigidBodySystem();
		
		void Run(float deltaTime);
	
		void setVisualToPXTransform();
		

		void setPxToVisualTransform();
		

		void OnAddEntity(Entity entity) override;
	

	};
}
