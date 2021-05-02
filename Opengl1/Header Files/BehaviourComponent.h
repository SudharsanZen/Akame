#pragma once
#include"Engine.h"
#include"Behaviour.h"

class BehaviourComponent
{
private:
	std::shared_ptr<Behaviour> behaviour;
	

public:
	
	template<typename T,typename ...Args>
	void setBehaviour(Args&&... args)
	{
		behaviour = std::make_shared<T>(std::forward<Args>(args)...);
	}

	void OnStart()
	{
		behaviour->OnStart();
	}
	
	void OnUpdate(float deltaTime)
	{
		behaviour->Update(deltaTime);
	}

	void reset()
	{
		behaviour.reset();
	}
};