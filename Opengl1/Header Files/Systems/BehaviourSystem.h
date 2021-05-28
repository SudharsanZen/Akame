#pragma once
#include"Engine.h"
#include"BehaviourComponent.h"


/*
* This class handels the BehaviourComponent Attached to An entity
*/
class BehaviourSystem :public System
{
	std::weak_ptr<ECS> ecs;
	friend class Scene;
public:

	//is called when Scene::OnStart() is called
	void OnStart()
	{
		// get each entity that has this system's signature and  process it
		for (auto const& ent : entities)
		{
			ecs.lock()->GetComponent<BehaviourComponent>(ent).OnStart(ecs);

		}
	}

	//is called when Scene::Render is called.
	void Update(float deltaTime)
	{
		// get each entity that has this system's signature and  process it
		for (auto const& ent : entities)
		{

			ecs.lock()->GetComponent<BehaviourComponent>(ent).OnUpdate(ecs,deltaTime);

		}
	}

	//this is called everytime a BehviourComponent is added to an Entity.
	void OnAddEntity() override
	{
		
	}

	
};