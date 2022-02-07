#pragma once
#include"Rendering/Camera.h"
#include<functional>
#include"ECS.h"
#include"Components/Behaviour/BehaviourComponent.h"
#include"Rendering/System/SceneTransformManager.h"
#include"Components/EntityDescriptor.h"
#include "Core/Editor/EntityDescriptionSystem.h"
#include "AkameCore.h"
class SkeletalMeshRenderingSystem;
class Window;
class RenderingSystem;
class BehaviourSystem;
class LightSystem;
class AnimationControllerSystem;
namespace physics 
{
	class RigidBodySystem;
}


class Scene
{
private:

	std::shared_ptr<ECS> ecs;
	
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
	friend class EntitySignatures;
	friend class EDFExporter;
public:
	
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
	T& AddComponent(Entity entityID);



	//Remove added component
	template<typename T>
	void RemoveComponent(Entity entityID);

	//get added component with a given entityID
	template<typename T>
	T& GetComponent(Entity entityID);

	//set backGround color of the window
	AKAME_API void backGroundColor(float r, float g, float b, float a);

	AKAME_API ~Scene();
	AKAME_API void release();
	AKAME_API float getDeltaTime();
};

template<typename T>
inline T& Scene::AddComponent(Entity entityID)
{

	T& comp=ecs->AddComponent<T>(entityID);
	comp.ecs = ecs;
	return comp;
}

//specialization of AddComponent for BehaviourComponent for storing entityID along with the component

//Remove added component

template<typename T>
inline void Scene::RemoveComponent(Entity entityID)
{
	ecs->RemoveComponent<T>(entityID);
}

//get added component with a given entityID

template<typename T>
inline T& Scene::GetComponent(Entity entityID)
{
	return ecs->GetComponent<T>(entityID);
}

