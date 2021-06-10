#pragma once
#include"Behaviour.h"
static float behvCount=0;
class BehaviourComponent
{
private:
	//Id of the entity to which this component belongs to
	EntityID eid;
	std::shared_ptr<Behaviour> behaviour;
	friend class BehaviourSystem;
	friend class Scene;
	
public:
	
	//set the Behaviour class to be used.
	template<typename T,typename ...Args>
	void setBehaviour(Args&&... args)
	{
		behaviour = std::make_shared<T>(std::forward<Args>(args)...); 
		behaviour->entityID = eid;
	}

	//called allong with Scene::OnStart()
	void OnStart(std::weak_ptr<ECS> ecs)
	{
		behaviour->ecs = ecs;
		behaviour->OnStart();
		
	}
	
	//called along with Scene::Render() every frame
	void OnUpdate(std::weak_ptr<ECS> ecs,float deltaTime)
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