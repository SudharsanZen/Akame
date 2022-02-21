#pragma once

#include"Physics\ColliderShape.h"
#include<memory>
#include<vector>
#include<cassert>
#include"Components\Components.h"

class ECS;
class ModelExporter;
class SceneDeserializer;
struct Entity;
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
		glm::vec3 velocity;
		physx::PxRigidActor* rigidbody;

		AKAME_API void ASSERT_RB();

		//detach all the shapes attached to the rigidbody
		AKAME_API void detachAllAttachedShapes();

		friend class RigidBodySystem;
		friend class ::SceneDeserializer;
		friend class ::ModelExporter;
	public:
		AKAME_API RigidBody3D();

		AKAME_API RigidBody3D(RigidBodyType rbType, ColliderShape shape);

		AKAME_API void setColliderShape(ColliderShape shape);

		AKAME_API void setVelocity(glm::vec3 vel);
		AKAME_API void setRigidBodyType(RigidBodyType rbType, ColliderShape shape);


		AKAME_API void ReleaseRbody();

		AKAME_API void reset();

	};

}