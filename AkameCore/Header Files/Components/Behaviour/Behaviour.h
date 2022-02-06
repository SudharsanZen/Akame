#pragma once
#include"ECS.h"
#include"Core/Reflection/ReflectionUIHandler.h"
#include"Components/EntityDescriptor.h"
class Behaviour
{
	std::weak_ptr<ECS> ecs;
	friend class BehaviourComponent;
	virtual void _serialize_data(ReflectionMeta& r) {  }
	virtual void _draw_data(ReflectionUIHandler& r) { }
	friend class InspectorWindow;
protected:

	Entity entityID;
	
	
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
		T& comp = ecs.lock()->AddComponent<T>(entityID);
		comp.eid = entityID;
		comp.ecs = ecs;
		return comp;
	}

	template<typename T>
	T& GetComponent(Entity& eid)
	{
		return ecs.lock()->GetComponent<T>(eid);
	}

	template<typename T>
	T& AddComponent(Entity& eid)
	{
		T& comp = ecs.lock()->AddComponent<T>(eid);
		comp.eid = eid;
		comp.ecs = ecs;
		return comp;
	}

	Entity CreateEntity()
	{
		Entity eid = ecs.lock()->CreateEntity();
		AddComponent<EntityDescriptor>(eid);
		return eid;

	}
};
