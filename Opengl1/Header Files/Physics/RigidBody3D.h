#pragma once
#include"ECS.h"
#include"Physics\Physics.h"
#include"Physics\ColliderShape.h"
#include<memory>
#include<vector>
namespace physics
{

	enum RigidBodyType
	{
		STATIC,DYNAMIC
	};

	class RigidBody3D
	{
		Entity eid;
		RigidBodyType rBodyType;
		ColliderShape colliderShape;
		std::weak_ptr<Physics> physicsPtr;
		std::weak_ptr<ECS> ecs;
		physx::PxRigidActor* rigidbody;

		void ASSERT_RB() { assert(rigidbody && "rigidbody is not initialized, trying to access nullptr!"); }

		//detach all the shapes attached to the rigidbody
		void detachAllAttachedShapes()
		{
			
			ASSERT_RB();
			physx::PxU32 n = rigidbody->getNbShapes();
			physx::PxShape** shapes = (physx::PxShape**)physx::platformAlignedAlloc(n * sizeof(physx::PxShape*));


			rigidbody->getShapes(shapes, rigidbody->getNbShapes(), 0);

			if (shapes)
			for (physx::PxU32 a = 0; a < n; a++)
			{
				rigidbody->detachShape(*shapes[a]);
			}

			physx::platformAlignedFree(shapes);
			
		}

		friend class RigidBodySystem;

	public:
		RigidBody3D()
		{
			rigidbody = NULL;
		}

		RigidBody3D(RigidBodyType rbType,ColliderShape shape)
		{
			setRigidBodyType(rbType,shape);
		}

		~RigidBody3D()
		{

		}
		void setColliderShape(ColliderShape shape)
		{
			colliderShape = shape;
			ASSERT_RB();
			physx::PxShape* colShape;
			detachAllAttachedShapes();
			std::shared_ptr<Physics> ptr = physicsPtr.lock();
			switch (shape.getShapeType())
			{
			case BOX:
				colShape=ptr->mPhysics->createShape(physx::PxBoxGeometry(shape.dim.x/2,shape.dim.y/2,shape.dim.z/2), *(ptr->mMaterial));
				break;
			case PLANE:
				colShape = ptr->mPhysics->createShape(physx::PxBoxGeometry(shape.dim.x/2, 0.1, shape.dim.y/2), *(ptr->mMaterial));
				break;
			case SPHERE:
				colShape = ptr->mPhysics->createShape(physx::PxSphereGeometry(shape.dim.x), *(ptr->mMaterial));
				break;
			default:
				std::cout <<"Invalid Collider Shape!";
			}
			if (!colShape)
			{
				std::cout <<"\nCan't attach collider shape to rigidBody!\n";
				return;
			}
			rigidbody->attachShape(*colShape);
			colShape->release();
		}

	
		void setRigidBodyType(RigidBodyType rbType, ColliderShape shape)
		{
			rBodyType = rbType;
			ReleaseRbody();
			
			if (rbType == DYNAMIC)
			{
				physx::PxTransform entityTransform = _ToPxTrans(ecs.lock()->GetComponent<Transform>(eid));
				rigidbody=(static_cast<physx::PxRigidActor*>(physicsPtr.lock()->mPhysics->createRigidDynamic(entityTransform)));
				
				setColliderShape(shape);
			}
			else if(rbType == STATIC)
			{
				physx::PxTransform entityTransform = _ToPxTrans(ecs.lock()->GetComponent<Transform>(eid));
				physx::PxRigidStatic* act=(physicsPtr.lock()->mPhysics->createRigidStatic(entityTransform));
				rigidbody = static_cast<physx::PxRigidActor*>(act);
				setColliderShape(shape);
			}
			else
			{
				std::cout <<"\nRigidBody3D::setRigidBodyType::passed invalid type!\n";
			}
			physicsPtr.lock()->mScene->addActor(*rigidbody);
		}


		void ReleaseRbody()
		{
			if (rigidbody)
			{
				rigidbody->release();
				rigidbody = NULL;
			}
		}

		void reset()
		{
			ReleaseRbody();
		}

	};

}