#pragma once
#include"Components/Behaviour/Behaviour.h"
#include"Components/Components.h"
#include"Scripting/BehaviourSystem.h"
#include"Core/AkameCore.h"
static float behvCount=0;
class BehaviourComponent:public Components
{
private:
	
	std::shared_ptr<Behaviour> behaviour;
	friend class BehaviourSystem;
	friend class Scene;
	friend class Behaviour;
	friend class InspectorWindow;
	
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

#ifdef AK_EXPORT
DEFINE_ECS_SPECIALIZATION_BHV(BehaviourComponent)
#endif