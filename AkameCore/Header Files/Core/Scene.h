#pragma once



#include"Rendering/Camera.h"
#include<functional>
#include"ECS.h"
#include"Components/Behaviour/BehaviourComponent.h"
#include"Rendering/System/SceneTransformManager.h"
#include"Components/EntityDescriptor.h"
#include "Core/Editor/EntityDescriptionSystem.h"
class Window;
class RenderingSystem;
class BehaviourSystem;
class LightSystem;
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
	
	glm::vec4 color;
	Window &window;

	float deltaTime;
	float lastTime;
	

	friend class BehaviourSystem;
	friend class Editor;
	
public:
	void vsyncOn(bool status);
	//fn will be called when Scene::Render() is called. this method will be dprecated
	std::function<void()> fn;
	//main camera
	Camera cam;
	Scene(Window& mainWindow);
	
	//call this to render a frame
	void clearBuffer();
	void Render();
	void swapBuffers();
	//call this before Scene::Render() in the main window loop 
	void OnStart();
	void InitEcs();

	//Creates an entity in the Scene
	EntityID CreateEntity()
	{
		EntityID eid=ecs->CreateEntity();
		AddComponent<EntityDescriptor>(eid);
		return eid;
	}
	void SetEntityTag(EntityID eid,std::string tag)
	{
		EDS->SetEntityTag(eid,tag);
	}
	void SetEntityName(EntityID eid,std::string name)
	{
		GetComponent<EntityDescriptor>(eid).SetName(name);
	}
	//Destroys an entity in the Scene
	void DestroyeEntity(EntityID entityID)
	{
		return ecs->DestroyEntity(entityID);
	}

	//Adds a component to the Given entity
	template<typename T>
	T& AddComponent(EntityID entityID);



	//Remove added component
	template<typename T>
	void RemoveComponent(EntityID entityID);

	//get added component with a given entityID
	template<typename T>
	T& GetComponent(EntityID entityID);

	//set backGround color of the window
	void backGroundColor(float r, float g, float b, float a)
	{
		this->color = glm::vec4(r, g, b, a);

	}

	~Scene()
	{
		release();
	
	}
	void release();
	float getDeltaTime()
	{
		return deltaTime;
	}
};

template<typename T>
inline T& Scene::AddComponent(EntityID entityID)
{

	T& comp=ecs->AddComponent<T>(entityID);
	comp.eid = entityID ;
	comp.ecs = ecs;
	return comp;
}

//specialization of AddComponent for BehaviourComponent for storing entityID along with the component



//Remove added component

template<typename T>
inline void Scene::RemoveComponent(EntityID entityID)
{
	ecs->RemoveComponent<T>(entityID);
}

//get added component with a given entityID

template<typename T>
inline T& Scene::GetComponent(EntityID entityID)
{
	return ecs->GetComponent<T>(entityID);
}


