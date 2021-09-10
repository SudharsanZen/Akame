#pragma once
#include"ECS.h"


class Behaviour
{
	std::weak_ptr<ECS> ecs;
	friend class BehaviourComponent;
protected:

	EntityID entityID;
	
	
public:

	
	
	virtual ~Behaviour() = default;
	virtual void OnStart() = 0;
	virtual void Update(float deltaTime) = 0;


	template<typename T>
	T& GetComponent()
	{
		return ecs.lock()->GetComponent<T>(entityID);
	}

	template<typename T>
	T& AddComponent(T comp)
	{
		return ecs.lock()->AddComponent<T>(entityID);
	}

	template<typename T>
	T& GetComponent(EntityID& eid)
	{
		return ecs.lock()->GetComponent<T>(eid);
	}

	template<typename T>
	T& AddComponent(EntityID& eid)
	{
		return ecs.lock()->AddComponent<T>(eid);
	}

	EntityID CreateEntity()
	{
		return ecs.lock()->CreateEntity();
	}
};
