#include "Physics\Physics.h"
#include"System.h"
#include "Physics\System\RigidBodySystem.h"
#include"Components\Physics\RigidBody3D.h"
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
		transform = _PxToTrans(rb3d.rigidbody->getGlobalPose());

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
		rb3d.eid = ent;
		rb3d.physicsPtr = mPhysics;
		rb3d.ecs = ecs;
	}
}