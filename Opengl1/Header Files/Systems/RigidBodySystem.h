#pragma once
#include"Engine.h"
#include"Physics/RigidBody3D.h"
class Scene;
namespace physics
{
	class RigidBodySystem :public System
	{
		std::shared_ptr<Physics> mPhysics;
		std::weak_ptr<ECS> ecs;
		friend class ::Scene;

	public:
		RigidBodySystem()
		{
			mPhysics = std::make_shared<Physics>();
			mPhysics->initPhysics();
		}

		void Run(float deltaTime)
		{
			setVisualToPXTransform();
			if (mPhysics->advance(deltaTime))
			{
				mPhysics->mScene->fetchResults(true);
				setPxToVisualTransform();
			}
		
				
			
		}

		void setVisualToPXTransform()
		{
			std::shared_ptr<ECS> e = ecs.lock();
			for (auto const& ent : entities)
			{

				RigidBody3D& rb3d = e->GetComponent<RigidBody3D>(ent);
				
				Transform& transform = e->GetComponent<Transform>(ent);
				rb3d.rigidbody->setGlobalPose(_ToPxTrans( transform));
		
			}
		}

		void setPxToVisualTransform()
		{
			glm::vec3 scale;
			std::shared_ptr<ECS> e = ecs.lock();
			for (auto const& ent : entities)
			{
				RigidBody3D& rb3d = e->GetComponent<RigidBody3D>(ent);
				Transform& transform = e->GetComponent<Transform>(ent);
				scale = transform.scale;
				transform = _PxToTrans(rb3d.rigidbody->getGlobalPose());
				
				transform.scale = scale;
				
			}
		
		}

		void OnAddEntity() override
		{
			std::shared_ptr<ECS> e = ecs.lock();
			for (auto const& ent : entities)
			{
				
				RigidBody3D& rb3d = e->GetComponent<RigidBody3D>(ent);
				rb3d.eid = ent;
				rb3d.physicsPtr = mPhysics;
				rb3d.ecs = ecs;
			}
		}

	};
}
