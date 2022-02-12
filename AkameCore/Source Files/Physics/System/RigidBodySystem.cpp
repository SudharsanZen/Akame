#include"System.h"
#include "Physics\System\RigidBodySystem.h"
#include"Components\Physics\RigidBody3D.h"
#include "Physics\Physics.h"
#pragma warning(push, 0)
#pragma warning( disable : 26812)
#pragma warning( disable : 26495)
#pragma warning( disable : 26451)
#pragma warning( disable : 33010)
#include<PxPhysicsAPI.h>
#pragma warning(pop)
#include"ECS.h"
physics::RigidBodySystem::RigidBodySystem()
{
	mPhysics = std::make_shared<Physics>();
	mPhysics->initPhysics();
}

void physics::RigidBodySystem::Run(float deltaTime)
{
	if (mPhysics->isAdvancing(deltaTime))
	{
		setVisualToPXTransform();
	}

	
		
	
	if (mPhysics->advance(deltaTime))
	{
		
		mPhysics->mScene->fetchResults(true);
		accumulator = 0;
		std::shared_ptr<ECS> e = ecs.lock();
		//collect initial tranform position for interpolation
		for (auto const& ent : entities)
		{

			RigidBody3D& rb3d = e->GetComponent<RigidBody3D>(ent);

			Transform& transform = e->GetComponent<Transform>(ent);
			transform.pxPoseInit = transform.GetGlobalPosition();
			transform.pxRotInit = transform.GetGlobalRotation().quaternion;

		}
	}

	accumulator += deltaTime;
	mixAmt = accumulator / mPhysics->mStepSize;
	mixAmt = glm::clamp(mixAmt, 0.0f, 1.0f);
	setPxToVisualTransform();



}

//set Physx transforms with Visual transform
void physics::RigidBodySystem::setVisualToPXTransform()
{
	std::shared_ptr<ECS> e = ecs.lock();
	for (auto const& ent : entities)
	{

		RigidBody3D& rb3d = e->GetComponent<RigidBody3D>(ent);

		Transform& transform = e->GetComponent<Transform>(ent);
		rb3d.rigidbody->setGlobalPose(_ToPxTrans(transform));

	}
}
//update the visual transform with the PhysX transform
void physics::RigidBodySystem::setPxToVisualTransform()
{
	glm::vec3 scale;
	std::shared_ptr<ECS> e = ecs.lock();
	for (auto const& ent : entities)
	{
		RigidBody3D& rb3d = e->GetComponent<RigidBody3D>(ent);
		Transform& transform = e->GetComponent<Transform>(ent);
		scale = transform.GetLocalScale();
		//_PxToTrans(rb3d.rigidbody->getGlobalPose(),transform,mixAmt);
		
		glm::vec3 newPose = glm::mix(transform.pxPoseInit, _PxToVec3(rb3d.rigidbody->getGlobalPose().p), mixAmt);
		glm::quat newRot = glm::mix(transform.pxRotInit, _PxToQuat(rb3d.rigidbody->getGlobalPose().q), mixAmt);
		transform.SetGlobalPosition(newPose);
		transform.SetGlobalRotation(newRot);
		
		transform.SetLocalScale(scale);

	}

}

//set pointers for ecs,physics when an entity is added the rbodysystem
void physics::RigidBodySystem::OnAddEntity(Entity entity)
{
	std::shared_ptr<ECS> e = ecs.lock();
	for (auto const& ent : entities)
	{

		RigidBody3D& rb3d = e->GetComponent<RigidBody3D>(ent);
		rb3d.physicsPtr = mPhysics;
		rb3d.ecs = ecs;
	}
}