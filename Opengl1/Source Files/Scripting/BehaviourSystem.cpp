#include"scripting/BehaviourSystem.h"
#include"ECS.h"

void BehaviourSystem::OnStart()
{
	// get each entity that has this system's signature and  process it
	for (auto const& ent : entities)
	{
		ecs.lock()->GetComponent<BehaviourComponent>(ent).OnStart(ecs);

	}
}

//is called when Scene::Render is called.
void BehaviourSystem::Update(float deltaTime)
{
	// get each entity that has this system's signature and  process it
	for (auto const& ent : entities)
	{

		ecs.lock()->GetComponent<BehaviourComponent>(ent).OnUpdate(ecs, deltaTime);

	}
}

//this is called everytime a BehviourComponent is added to an Entity.
void BehaviourSystem::OnAddEntity(Entity entity)
{

}