#pragma once
#include"Rendering/Camera.h"
#include<functional>
#define DECLARE_ECS_SPECIALIZATION_SCENE(type)					\
	template<>													\
	AKAME_API type& AddComponent<type>(Entity entityID);		\
	template<>													\
	AKAME_API void RemoveComponent<type>(Entity entityID);		\
	template<>													\
	AKAME_API type& GetComponent<type>(Entity entityID);		\
	template<>													\
	AKAME_API bool IsComponentAttached<type>(Entity entityID);  

#if defined(AK_EXPORT) || defined(AK_STATIC)
#include"ECS.h"
#define DEFINE_ECS_SPECIALIZATION_SCENE(type)					\
	template<>													\
	type& Scene::AddComponent<type>(Entity entityID)			\
	{															\
		type& comp = ecs->AddComponent<type>(entityID);			\
		comp.ecs = ecs;											\
		return comp;											\
	}															\
	template<>													\
	void Scene::RemoveComponent<type>(Entity entityID)			\
	{															\
		ecs->RemoveComponent<type>(entityID);					\
	}															\
	template<>													\
	type& Scene::GetComponent<type>(Entity entityID)			\
	{															\
		return ecs->GetComponent<type>(entityID);				\
	}															\
	template<>													\
	bool Scene::IsComponentAttached<type>(Entity entityID)		\
	{															\
		return ecs->IsComponentAttached<type>(entityID);		\
	}
#else
class ECS;
#include"ECSTYPES.h"
#endif

#include"Rendering/System/SceneTransformManager.h"

#include "Core/Editor/EntityDescriptionSystem.h"
#include "AkameCore.h"
#include"Components/Behaviour/BehaviourComponent.h"
class SkeletalMeshRenderingSystem;
class Window;
class RenderingSystem;
class BehaviourSystem;
class LightSystem;
class AnimationControllerSystem;
class SceneEntityListSystem;
namespace physics 
{
	class RigidBodySystem;
}


class Scene
{
private:

	std::shared_ptr<ECS> ecs;
	
	std::shared_ptr<SceneEntityListSystem> e_list_system;
	std::shared_ptr<RenderingSystem> renderSys;
	std::shared_ptr<BehaviourSystem> behaviourSys;
	std::shared_ptr<LightSystem> lightSys;
	std::shared_ptr<physics::RigidBodySystem> physicsSys;
	std::shared_ptr<SceneTransformManager> transformManager;
	std::shared_ptr<EntityDescriptionSystem> EDS;
	std::shared_ptr<SkeletalMeshRenderingSystem> animSys;
	std::shared_ptr<AnimationControllerSystem> animContSys;
	glm::vec4 color;
	Window &window;

	float deltaTime=0.0f;
	float lastTime=0.0f,currTime=0.0f;
	

	friend class BehaviourSystem;
	friend class Exporter;
	friend class Editor;
	friend class LightAndShadowConfig;
	friend class SceneHierarchyWindow;
	friend class ViewPortWindow;
	friend class Exporter;
	friend class InspectorWindow;
	friend class Exporter;
	friend struct EntitySignatures;
	friend class SceneDeserializer;
	friend class EDFExporter;
	AKAME_API void InitScene();
public:
#ifdef AK_PRIVATE_GETTER_SETTER
	template<typename _sysType>
	std::shared_ptr<_sysType> GetSystem()
	{
		return ecs->systemManager.GetSystemPtr<_sysType>();
	}
	std::shared_ptr<ECS> GetECS()
	{
		return ecs;
	}
#endif
	AKAME_API void groupEntWithShader();
	AKAME_API void vsyncOn(bool status);
	//fn will be called when Scene::Render() is called. this method will be dprecated
	std::function<void()> fn;
	//main camera
	Camera cam;
	AKAME_API Scene(Window& mainWindow);

	
	//call this to render a frame
	AKAME_API void clearBuffer();
	AKAME_API void Render();
	AKAME_API void swapBuffers();
	//call this before Scene::Render() in the main window loop 
	AKAME_API void OnStart();
	AKAME_API void InitEcs();

	//Creates an entity in the Scene
	AKAME_API Entity CreateEntity();
	AKAME_API void SetEntityTag(Entity eid, std::string tag);
	AKAME_API void SetEntityName(Entity eid, std::string name);
	//Destroys an entity in the Scene
	AKAME_API void DestroyeEntity(Entity entityID);

	//Adds a component to the Given entity
	template<typename T>
	AKAME_API T& AddComponent(Entity entityID);
	//Remove added component
	template<typename T>
	AKAME_API void RemoveComponent(Entity entityID);
	//get added component with a given entityID
	template<typename T>
	AKAME_API T& GetComponent(Entity entityID);
	template<typename T>
	AKAME_API bool IsComponentAttached(Entity entityID);

	DECLARE_ECS_SPECIALIZATION_SCENE(Transform)
	DECLARE_ECS_SPECIALIZATION_SCENE(Mesh)
	DECLARE_ECS_SPECIALIZATION_SCENE(BehaviourComponent)
	DECLARE_ECS_SPECIALIZATION_SCENE(AnimationController)
	DECLARE_ECS_SPECIALIZATION_SCENE(physics::RigidBody3D)
	DECLARE_ECS_SPECIALIZATION_SCENE(SkeletalMesh)
	DECLARE_ECS_SPECIALIZATION_SCENE(Material)
	DECLARE_ECS_SPECIALIZATION_SCENE(EntityDescriptor)
	DECLARE_ECS_SPECIALIZATION_SCENE(Lights)

	//set backGround color of the window
	AKAME_API void backGroundColor(float r, float g, float b, float a);

	AKAME_API ~Scene();
	AKAME_API void release();
	AKAME_API float getDeltaTime();
};


#if defined(AK_EXPORT) || defined(AK_STATIC)

template<typename T>
T& Scene::AddComponent(Entity entityID)
{

	T& comp=ecs->AddComponent<T>(entityID);
	comp.ecs = ecs;
	return comp;
}
template<typename T>
bool Scene::IsComponentAttached(Entity eid)
{
	ecs->IsComponentAttached<T>(eid);
}
//Remove added component
template<typename T>
void Scene::RemoveComponent(Entity entityID)
{
	ecs->RemoveComponent<T>(entityID);
}
//get added component with a given entityID
template<typename T>
T& Scene::GetComponent(Entity entityID)
{
	return ecs->GetComponent<T>(entityID);
}
DEFINE_ECS_SPECIALIZATION_SCENE(Transform)
DEFINE_ECS_SPECIALIZATION_SCENE(Lights)
DEFINE_ECS_SPECIALIZATION_SCENE(Mesh)
DEFINE_ECS_SPECIALIZATION_SCENE(BehaviourComponent)
DEFINE_ECS_SPECIALIZATION_SCENE(AnimationController)
DEFINE_ECS_SPECIALIZATION_SCENE(physics::RigidBody3D)
DEFINE_ECS_SPECIALIZATION_SCENE(SkeletalMesh)
DEFINE_ECS_SPECIALIZATION_SCENE(Material)
DEFINE_ECS_SPECIALIZATION_SCENE(EntityDescriptor)
#endif