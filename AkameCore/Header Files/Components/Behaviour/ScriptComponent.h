#pragma once

#include"Components/Components.h"
#include"Behaviour.h"
#include"Core/Scene.h"



class ScriptComponent:public Components
{
private:

	std::shared_ptr<Behaviour> behaviour;
	friend class BehaviourSystem;
	friend class Scene;
	friend class Behaviour;
	friend class InspectorWindow;
	friend class ScriptableSystem;
	std::string comp_name;

public:

	//set the Behaviour class to be used.

	void setBehaviour(std::shared_ptr<Behaviour> beh)
	{
		if (beh)
		{
			behaviour = beh;
			behaviour->entityID = eid;
		}
		else
		{
			std::cout <<"null pointer returned!";
		}
	}

	//called allong with Scene::OnStart()
	void OnStart(std::weak_ptr<ECS> ecs)
	{
		behaviour->ecs = ecs;
		behaviour->OnStart();

	}

	//called along with Scene::Render() every frame
	void OnUpdate(std::weak_ptr<ECS> ecs, float deltaTime)
	{
		behaviour->ecs = ecs;
		behaviour->Update(deltaTime);
	}

	//resets the behaviour pointer
	void reset()
	{
		behaviour.reset();
	}


	/*~BehaviourComponent()
	{
		std::cout <<"\n~BehaviourCOmponent()\n";
		behvCount++;
		std::cout << "count:" << behvCount;
	}*/
};