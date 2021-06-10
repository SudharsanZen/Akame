#include "Physics\Physics.h"
#include"System.h"
#include "Systems\RigidBodySystem.h"
#include"Physics\RigidBody3D.h"
#include<PxPhysicsAPI.h>
#include"ECS.h"
physics::RigidBodySystem::RigidBodySystem()
{
	mPhysics = std::make_shared<Physics>();
	mPhysics->initPhysics();
}

void physics::RigidBodySystem::Run(float deltaTime)
{
	setVisualToPXTransform();
	if (mPhysics->advance(deltaTime))
	{
		mPhysics->mScene->fetchResults(true);
		setPxToVisualTransform();
	}



}

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

void physics::RigidBodySystem::setPxToVisualTransform()
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

void physics::RigidBodySystem::OnAddEntity()
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