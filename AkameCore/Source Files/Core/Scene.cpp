#include"Core/Editor/EditorUI.h"
#include "Core/Scene.h"
#include"stb_image.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Components/Rendering/Mesh.h"
#include"Components/Rendering/Material.h"
#include"Components/Physics/RigidBody3D.h"
#include"Components/Animation/SkeletalMesh.h"
#include"Animation/SkeletalMeshRenderingSystem.h"
#include"Core/Window.h"
#include"Rendering/System/RenderingSystem.h"
#include"Scripting/BehaviourSystem.h"
#include"Physics/System/RigidBodySystem.h"
#include"ECS.h"
#include"Core/Log/Log.h"
#include"Rendering/System/LightSystem.h"
#include"misc/temp.h"
#include"Core/Debug/Debug.h"
#include"Animation/AnimationControllerSystem.h"
std::vector<std::shared_ptr<RenderingSystem>> listScene;


void callBack(GLFWwindow* window,int w,int h)
{
	glViewport(0, 0, w, h);
	for (auto &f : listScene)
	{
		if (!f->EditorMode())
		{
			f->height = h;
			f->width = w;
			f->updateFrameBufferSize(h, w);
		}
		

		
		
	}

	
}
void Scene::groupEntWithShader()
{
	renderSys->GroupEntityWithCommonShader();
}
void Scene::vsyncOn(bool status)
{
	if (status)
	{
		glfwSwapInterval(1);
	}
	else
	{
		glfwSwapInterval(0);
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
	color = glm::vec4(0,0,0,1);
	cam.setFieldOfView(60.0f);
	cam.transform.SetGlobalPosition(glm::vec3(5, 5, 5));
	cam.transform.SetLocalRotation(Quaternion::rotationAroundAxisVector(-135, worldUp));
	cam.transform.SetLocalRotation(Quaternion::rotationAroundAxisVector(-45, cam.transform.right()) * cam.transform.GetGlobalRotation());
	InitEcs();
	if (!AssetManager::isInitialized())
	{
		AssetManager::init();
	}
	glfwSwapInterval(0);
	renderSys->updateFrameBufferSize(mainWindow.getBufferHeight(),mainWindow.getBufferWidth());
	mainWindow.setBufferSizeCallBackFunction(callBack);
	listScene.push_back(renderSys);
	renderSys->height = window.getBufferHeight();
	renderSys->width = window.getBufferWidth();
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
	ecs->RegisterComponent<EntityDescriptor>();
	ecs->RegisterComponent<SkeletalMesh>();
	ecs->RegisterComponent<AnimationController>();

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

	Signature transformSig;
	transformSig.set(ecs->GetComponentBitPose<Transform>());

	Signature entityDescriptor;
	entityDescriptor.set(ecs->GetComponentBitPose<EntityDescriptor>());

	Signature animSig;
	animSig.set(ecs->GetComponentBitPose<SkeletalMesh>());
	animSig.set(ecs->GetComponentBitPose<Transform>());
	animSig.set(ecs->GetComponentBitPose<Material>());

	Signature animCont;
	animCont.set(ecs->GetComponentBitPose<AnimationController>());

	//register system and it's signature
	renderSys=ecs->RegisterSystem<RenderingSystem>();
	behaviourSys=ecs->RegisterSystem<BehaviourSystem>();
	physicsSys = ecs->RegisterSystem<physics::RigidBodySystem>();
	lightSys = ecs->RegisterSystem<LightSystem>();
	transformManager = ecs->RegisterSystem<SceneTransformManager>();
	EDS = ecs->RegisterSystem<EntityDescriptionSystem>();
	animSys = ecs->RegisterSystem<SkeletalMeshRenderingSystem>();
	animContSys = ecs->RegisterSystem<AnimationControllerSystem>();

	//setting some member attributes of the systems
	behaviourSys->ecs = ecs;
	physicsSys->ecs=ecs;
	renderSys->lightsystem = lightSys;
	renderSys->animSys = animSys;
	lightSys->ecs = ecs;
	renderSys-> ecs = ecs;
	transformManager->ecs = ecs;
	animSys->ecs = ecs;
	animContSys->ecs = ecs;
	EDS->ecs = ecs;

	//registering system signature
	ecs->SetSystemSignature<EntityDescriptionSystem>(entityDescriptor);
	ecs->SetSystemSignature<RenderingSystem>(renderSysSig);
	ecs->SetSystemSignature<BehaviourSystem>(behSysSig);
	ecs->SetSystemSignature<physics::RigidBodySystem>(physicsSysSig);
	ecs->SetSystemSignature<LightSystem>(lightingSysSig);
	ecs->SetSystemSignature<SceneTransformManager>(transformSig);
	ecs->SetSystemSignature<SkeletalMeshRenderingSystem>(animSig);
	ecs->SetSystemSignature<AnimationControllerSystem>(animCont);


}



void Scene::release()
{
	ENGINE_CORE_INFO("~Scene()");
	physicsSys.reset();
	ecs.reset();
	behaviourSys.reset();
	renderSys.reset();
	lightSys.reset();
	EDS.reset();
	transformManager.reset();
}

void Scene::clearBuffer()
{
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glClearColor(1,1,1,1.0f);

	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	window.processInput();
	glfwPollEvents();
}

/*Render a Frame of the Scene.
* call this inside the main window loop. 
*/
void Scene::Render()
{
	
		currTime = glfwGetTime();
	//renderStuff
		cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		//call this in this same order
		EDS->updateMap();
		behaviourSys->Update(deltaTime);
		animSys->Run();
		animContSys->update(deltaTime);
		transformManager->UpdateTransforms();
		renderSys->Run(cam);
		physicsSys->Run(deltaTime);
		
		fn();
	
	deltaTime = currTime - lastTime;
	lastTime = currTime;

	
	
}

void Scene::swapBuffers()
{
	
	window.swapBuffers();
	
}
