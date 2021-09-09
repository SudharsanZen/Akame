#pragma once

#include"Physics\ColliderShape.h"
#include<memory>
#include<vector>
#include<cassert>
#include"Components\Components.h"
class ECS;
typedef std::size_t Entity;
namespace physx
{
	class PxRigidActor;
}
namespace physics
{
	class Physics;

	enum class RigidBodyType
	{
		STATIC,DYNAMIC
	};

	class RigidBody3D:public Components
	{

		RigidBodyType rBodyType;
		ColliderShape colliderShape;
		std::weak_ptr<Physics> physicsPtr;

		physx::PxRigidActor* rigidbody;

		void ASSERT_RB() { assert(rigidbody && "rigidbody is not initialized, trying to access nullptr!"); }

		//detach all the shapes attached to the rigidbody
		void detachAllAttachedShapes();

		friend class RigidBodySystem;

	public:
		RigidBody3D()
		{
			eid = EntityID(-1,-1);
			rBodyType = RigidBodyType::STATIC;
			rigidbody = NULL;
		}

		RigidBody3D(RigidBodyType rbType,ColliderShape shape)
		{
			setRigidBodyType(rbType,shape);
		}

		void setColliderShape(ColliderShape shape);

	
		void setRigidBodyType(RigidBodyType rbType, ColliderShape shape);


		void ReleaseRbody();

		void reset()
		{
			ReleaseRbody();
		}

	};

}