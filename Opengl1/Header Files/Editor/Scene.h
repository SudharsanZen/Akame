#pragma once
#include"ECS.h"
#include"Engine.h"
#include"Systems/RenderingSystem.h"
#include"Systems/BehaviourSystem.h"
#include"BehaviourComponent.h"

class Scene
{
private:

	std::shared_ptr<ECS> ecs;
	std::shared_ptr<RenderingSystem> renderSys;
	std::shared_ptr<BehaviourSystem> behaviourSys;



	Window &window;
	GLuint uboMatrixBufferID;
	GLsizeiptr mat4Size = sizeof(glm::mat4);
	float deltaTime;
	float lastTime;
	Camera cam;
	void updateUniformBuffer(Camera& cam);
	void flyCam(Camera& cam, Window& window);
	friend class BehaviourSystem;
public:
	Scene(Window &mainWindow):cam(60, 1, 0.1f, 1000),window(mainWindow)
	{
		deltaTime = 0;
		lastTime = 0;

		stbi_set_flip_vertically_on_load(true);
	
		//intialize global uniform buffer for storing projection and view matrix
		glGenBuffers(1, &uboMatrixBufferID);
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrixBufferID);
		glBufferData(GL_UNIFORM_BUFFER, 2 * mat4Size, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrixBufferID, 0, 2 * mat4Size);
		
		cam.setFieldOfView(60.0f);
		cam.transform.position = glm::vec3(5, 5, 5);
		InitEcs();
		glfwSwapInterval(0);
	}
	void Render();

	void InitEcs();
	
	EntityID CreateEntity()
	{
		return ecs->CreateEntity();
	}

	void DestroyeEntity(EntityID entityID)
	{
		return ecs->DestroyEntity(entityID);
	}

	template<typename T>
	void AddComponent(EntityID entityID,T comp)
	{
		ecs->AddComponent<T>(entityID, comp);
	}

	template<typename T>
	void RemoveComponent(EntityID entityID)
	{
		ecs.RemoveComponent<T>(entityID);
	}

	template<typename T>
	T& GetComponent(EntityID entityID)
	{
		return ecs->GetComponent<T>(entityID);
	}


};

