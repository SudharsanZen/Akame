#include "Components\Physics\RigidBody3D.h"
#include<PxPhysicsAPI.h>
#include"Physics/Physics.h"
#include"ECS.h"

void physics::RigidBody3D::detachAllAttachedShapes()
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

void physics::RigidBody3D::setColliderShape(ColliderShape shape)
{
	colliderShape = shape;
	ASSERT_RB();
	physx::PxShape* colShape=nullptr;
	detachAllAttachedShapes();
	std::shared_ptr<Physics> ptr = physicsPtr.lock();
	switch (shape.getShapeType())
	{
	case Shapes::BOX:
		colShape = ptr->mPhysics->createShape(physx::PxBoxGeometry(shape.dim.x / 2, shape.dim.y / 2, shape.dim.z / 2), *(ptr->mMaterial));
		break;
	case Shapes::PLANE:
		colShape = ptr->mPhysics->createShape(physx::PxBoxGeometry(shape.dim.x / 2, 0.01f, shape.dim.y / 2), *(ptr->mMaterial));
		break;
	case Shapes::SPHERE:
		colShape = ptr->mPhysics->createShape(physx::PxSphereGeometry(shape.dim.x), *(ptr->mMaterial));
		break;
	default:
		std::cout << "Invalid Collider Shape!";
	}
	if (!colShape)
	{
		std::cout << "\nCan't attach collider shape to rigidBody!\n";
		return;
	}
	rigidbody->attachShape(*colShape);
	colShape->release();
}

void physics::RigidBody3D::setRigidBodyType(RigidBodyType rbType, ColliderShape shape)
{
	if (rigidbody)
	{
		rigidbody->release();
		rigidbody = NULL;
	}

	rBodyType = rbType;
	ReleaseRbody();

	if (rbType == RigidBodyType::DYNAMIC)
	{
		physx::PxTransform entityTransform = _ToPxTrans(ecs.lock()->GetComponent<Transform>(eid));
		rigidbody = (static_cast<physx::PxRigidActor*>(physicsPtr.lock()->mPhysics->createRigidDynamic(entityTransform)));

		setColliderShape(shape);
	}
	else if (rbType == RigidBodyType::STATIC)
	{
		physx::PxTransform entityTransform = _ToPxTrans(ecs.lock()->GetComponent<Transform>(eid));
		physx::PxRigidStatic* act = (physicsPtr.lock()->mPhysics->createRigidStatic(entityTransform));
		rigidbody = static_cast<physx::PxRigidActor*>(act);
		setColliderShape(shape);
	}
	else
	{
		std::cout << "\nRigidBody3D::setRigidBodyType::passed invalid type!\n";
	}
	physicsPtr.lock()->mScene->addActor(*rigidbody);
}

void physics::RigidBody3D::ReleaseRbody()
{
	if (rigidbody)
	{
		rigidbody->release();
		rigidbody = NULL;
	}
}