#include "Editor/Scene.h"
#include"stb_image.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Mesh.h"
#include"Material.h"
#include"Physics/RigidBody3D.h"
#include"Window.h"
#include"Systems/RenderingSystem.h"
#include"Systems/BehaviourSystem.h"
#include"Systems/RigidBodySystem.h"
#include"ECS.h"
#include"Log/Log.h"
#include"Systems/LightSystem.h"
#include"temp.h"
std::vector<std::shared_ptr<RenderingSystem>> listScene;


void callBack(GLFWwindow* window,int w,int h)
{
	glViewport(0, 0, w, h);
	for (auto &f : listScene)
	{
		f->updateFrameBufferSize(h,w);
	}

	
}
//constructor initia
Scene::Scene(Window &mainWindow) :cam(60, 1, 0.1f, 1000), window(mainWindow)
{
	ENGINE_CORE_INFO("INITIALIZING SCENE");
	//initialize fn to an lambda function with no body
	fn = []() {};

	deltaTime = 0;
	lastTime = 0;

	stbi_set_flip_vertically_on_load(true);

	cam.setFieldOfView(60.0f);
	cam.transform.position = glm::vec3(5, 5, 5);
	InitEcs();
	if (!AssetManager::isInitialized())
	{
		AssetManager::init();
	}
	glfwSwapInterval(0);
	renderSys->updateFrameBufferSize(mainWindow.getBufferHeight(),mainWindow.getBufferWidth());
	mainWindow.setBufferSizeCallBackFunction(callBack);
	listScene.push_back(renderSys);
}




//call this before Scene::Render() in the main window loop 


//call this before Scene::Render() in the main window loop 

void Scene::OnStart()
{
	behaviourSys->OnStart();
}

//Initialize the Entity component System
void Scene::InitEcs()
{
	ecs = std::make_shared<ECS>();

	//register components
	ecs->RegisterComponent<Mesh>();
	ecs->RegisterComponent<Transform>();
	ecs->RegisterComponent<Material>();
	ecs->RegisterComponent<BehaviourComponent>();
	ecs->RegisterComponent<physics::RigidBody3D>();
	ecs->RegisterComponent<Lights>();


	//create system signature
	Signature renderSysSig;
	renderSysSig.set(ecs->GetComponentBitPose<Mesh>(),true);
	renderSysSig.set(ecs->GetComponentBitPose<Transform>(),true);
	renderSysSig.set(ecs->GetComponentBitPose<Material>(),true);

	Signature behSysSig;
	behSysSig.set(ecs->GetComponentBitPose<BehaviourComponent>(),true);

	Signature physicsSysSig;
	physicsSysSig.set(ecs->GetComponentBitPose<Transform>());
	physicsSysSig.set(ecs->GetComponentBitPose<physics::RigidBody3D>());

	Signature lightingSysSig;
	lightingSysSig.set(ecs->GetComponentBitPose<Lights>());
	lightingSysSig.set(ecs->GetComponentBitPose<Transform>());

	//register system and it's signature
	renderSys=ecs->RegisterSystem<RenderingSystem>();
	behaviourSys=ecs->RegisterSystem<BehaviourSystem>();
	physicsSys = ecs->RegisterSystem<physics::RigidBodySystem>();
	lightSys = ecs->RegisterSystem<LightSystem>();
	behaviourSys->ecs = ecs;
	physicsSys->ecs=ecs;
	renderSys->lightsystem = lightSys;
	lightSys->ecs = ecs;

	ecs->SetSystemSignature<RenderingSystem>(renderSysSig);
	ecs->SetSystemSignature<BehaviourSystem>(behSysSig);
	ecs->SetSystemSignature<physics::RigidBodySystem>(physicsSysSig);
	ecs->SetSystemSignature<LightSystem>(lightingSysSig);


}



void Scene::release()
{
	ENGINE_CORE_INFO("~Scene()");
	physicsSys.reset();
	ecs.reset();
	behaviourSys.reset();
	renderSys.reset();
}

/*Render a Frame of the Scene.
* call this inside the main window loop. 
*/
void Scene::Render()
{
	
	float currTime = glfwGetTime();

	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glClearColor(1,1,1,1.0f);
	glClearColor(color.x,color.y,color.z,color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	//renderStuff
		renderSys->Run(ecs,cam);
		behaviourSys->Update(deltaTime);
		physicsSys->Run(deltaTime);
		fn();
	
		//lines(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)).renderMesh();
		//lines(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)).renderMesh();
		//lines(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1)).renderMesh();

	window.processInput();
	window.swapBuffers();
	glfwPollEvents();
	deltaTime = currTime - lastTime;
	lastTime = currTime;
}
