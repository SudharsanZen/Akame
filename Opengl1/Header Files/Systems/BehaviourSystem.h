#pragma once
#include"Engine.h"
#include"BehaviourComponent.h"

class BehaviourSystem :public System
{

public:
	void OnStart(std::shared_ptr<ECS> ecs)
	{
		for (auto const& ent : entities)
		{
			ecs->GetComponent<BehaviourComponent>(ent).OnStart();

		}
	}
	void Update(std::shared_ptr<ECS> ecs, float deltaTime)
	{
		for (auto const& ent : entities)
		{

			ecs->GetComponent<BehaviourComponent>(ent).OnUpdate(deltaTime);

		}
	}

};