#pragma once
#include"Editor/Scene.h"


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
	void AddComponent(T comp)
	{
		ecs.lock()->AddComponent<T>(entityID, comp);
	}

	template<typename T>
	T& GetComponent(EntityID& eid)
	{
		return ecs.lock()->GetComponent<T>(eid);
	}

	template<typename T>
	void AddComponent(EntityID& eid,T comp)
	{
		ecs.lock()->AddComponent<T>(eid,comp);
	}
};
