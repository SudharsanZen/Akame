#pragma once
#include"Core/Reflection/ReflectionUIHandler.h"
#include"Components/EntityDescriptor.h"
#include"Components/Rendering/Mesh.h"
#include"Components/Rendering/Material.h"
#include"Components/Physics/RigidBody3D.h"
#include"Components/Animation/SkeletalMesh.h"
#include"Components/Animation/AnimationController.h"
#include"Components/Lighting/Lights.h"
#include"Core\AkameCore.h"
#define DECLARE_ECS_SPECIALIZATION_BHV(type)					\
	template<>													\
	AKAME_API type& AddComponent<type>(Entity eid);				\
	template<>													\
	AKAME_API type& AddComponent<type>();						\
	template<>													\
	AKAME_API void RemoveComponent<type>(Entity eid);			\
	template<>													\
	AKAME_API void RemoveComponent<type>();						\
	template<>													\
	AKAME_API type& GetComponent<type>(Entity eid);				\
	template<>													\
	AKAME_API type& GetComponent<type>();

#if defined(AK_EXPORT) || defined(AK_STATIC)
#include"ECS.h"
#define DEFINE_ECS_SPECIALIZATION_BHV(type)						\
	template<>													\
	type& Behaviour::AddComponent<type>(Entity eid)				\
	{															\
		type& comp = ecs.lock()->AddComponent<type>(eid);		\
		comp.eid = eid;											\
		comp.ecs = ecs;											\
		return comp;											\
	}															\
	template<>													\
	type& Behaviour::AddComponent<type>()						\
	{															\
		type& comp = ecs.lock()->AddComponent<type>(entityID);	\
		comp.eid = entityID;									\
		comp.ecs = ecs;											\
		return comp;											\
	}															\
	template<>													\
	void Behaviour::RemoveComponent<type>(Entity eid)			\
	{															\
		ecs.lock()->RemoveComponent<type>(eid);					\
	}															\
	template<>													\
	void Behaviour::RemoveComponent<type>()						\
	{															\
		ecs.lock()->RemoveComponent<type>(entityID);			\
	}															\
	template<>													\
	type& Behaviour::GetComponent<type>(Entity eid)				\
	{															\
		return ecs.lock()->GetComponent<type>(eid);				\
	}															\
	template<>													\
	type& Behaviour::GetComponent<type>()						\
	{															\
		return ecs.lock()->GetComponent<type>(entityID);		\
	}
#else
class ECS;
#include"ECSTYPES.h"
#endif
class BehaviourComponent;
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
	AKAME_API T& GetComponent();

	template<typename T>
	AKAME_API T& AddComponent();

	template<typename T>
	AKAME_API T& GetComponent(Entity eid);
	
	template<typename T>
	AKAME_API T& AddComponent(Entity eid);
	template<typename T>
	AKAME_API void RemoveComponent();
	template<typename T>
	AKAME_API void RemoveComponent(Entity eid);

	DECLARE_ECS_SPECIALIZATION_BHV(Transform)
	DECLARE_ECS_SPECIALIZATION_BHV(Mesh)
	DECLARE_ECS_SPECIALIZATION_BHV(BehaviourComponent)
	DECLARE_ECS_SPECIALIZATION_BHV(AnimationController)
	DECLARE_ECS_SPECIALIZATION_BHV(physics::RigidBody3D)
	DECLARE_ECS_SPECIALIZATION_BHV(SkeletalMesh)
	DECLARE_ECS_SPECIALIZATION_BHV(Material)
	DECLARE_ECS_SPECIALIZATION_BHV(EntityDescriptor)
	DECLARE_ECS_SPECIALIZATION_BHV(Lights)



	AKAME_API Entity CreateEntity();
};

#if defined(AK_EXPORT) || defined(AK_STATIC)

template<typename T>
T& Behaviour::GetComponent()
{
	return ecs.lock()->GetComponent<T>(entityID);
}

template<typename T>
T& Behaviour::AddComponent()
{
	T& comp = ecs.lock()->AddComponent<T>(eid);
	comp.eid = entityID;
	comp.ecs = ecs;
	return comp;
}

template<typename T>
T& Behaviour::GetComponent(Entity eid)
{
	return ecs.lock()->GetComponent<T>(eid);
}

template<typename T>
T& Behaviour::AddComponent(Entity eid)
{
	T& comp = ecs.lock()->AddComponent<T>(eid);
	comp.eid = eid;
	comp.ecs = ecs;
	return comp;
}
DEFINE_ECS_SPECIALIZATION_BHV(Transform)
DEFINE_ECS_SPECIALIZATION_BHV(Lights)
DEFINE_ECS_SPECIALIZATION_BHV(Mesh)
DEFINE_ECS_SPECIALIZATION_BHV(AnimationController)
DEFINE_ECS_SPECIALIZATION_BHV(physics::RigidBody3D)
DEFINE_ECS_SPECIALIZATION_BHV(SkeletalMesh)
DEFINE_ECS_SPECIALIZATION_BHV(Material)
DEFINE_ECS_SPECIALIZATION_BHV(EntityDescriptor)
#endif