#pragma once
#include"ECS.h"
#include"Engine.h"
#include"Systems/RenderingSystem.h"
#include"Systems/BehaviourSystem.h"
#include"Systems/RigidBodySystem.h"


class Scene
{
private:

	std::shared_ptr<ECS> ecs;

	std::shared_ptr<RenderingSystem> renderSys;
	std::shared_ptr<BehaviourSystem> behaviourSys;
	std::shared_ptr<physics::RigidBodySystem> physicsSys;

	glm::vec4 color;
	Window &window;
	GLuint uboMatrixBufferID;
	GLsizeiptr mat4Size = sizeof(glm::mat4);
	float deltaTime;
	float lastTime;
	
	void updateUniformBuffer(Camera& cam);
	friend class BehaviourSystem;
public:
	//fn will be called when Scene::Render() is called. this method will be dprecated
	std::function<void()> fn;
	//main camera
	Camera cam;
	Scene(Window& mainWindow);
	
	//call this to render a frame
	void Render();

	//call this before Scene::Render() in the main window loop 
	void OnStart()
	{
		behaviourSys->OnStart();
	}
	void InitEcs();

	//Creates an entity in the Scene
	EntityID CreateEntity()
	{
		return ecs->CreateEntity();
	}

	//Destroys an entity in the Scene
	void DestroyeEntity(EntityID entityID)
	{
		return ecs->DestroyEntity(entityID);
	}

	//Adds a component to the Given entity
	template<typename T>
	void AddComponent(EntityID entityID,T comp)
	{
		
		ecs->AddComponent<T>(entityID, comp);
	}

	//specialization of AddComponent for BehaviourComponent for storing entityID along with the component
	template<>
	void AddComponent<BehaviourComponent>(EntityID entityID, BehaviourComponent comp)
	{
		comp.eid = entityID;
		ecs->AddComponent<BehaviourComponent>(entityID, comp);
	}

	//Remove added component
	template<typename T>
	void RemoveComponent(EntityID entityID)
	{
		ecs->RemoveComponent<T>(entityID);
	}

	//get added component with a given entityID
	template<typename T>
	T& GetComponent(EntityID entityID)
	{
		return ecs->GetComponent<T>(entityID);
	}

	//set backGround color of the window
	void backGroundColor(float r,float g,float b,float a)
	{
		this->color =glm::vec4(r,g,b,a);
	}

	~Scene()
	{
		std::cout <<"\n~Scene()\n";
	}

	float getDeltaTime()
	{
		return deltaTime;
	}
};

