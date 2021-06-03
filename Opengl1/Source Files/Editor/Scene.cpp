#include "Editor/Scene.h"


//constructor initia
Scene::Scene(Window &mainWindow) :cam(60, 1, 0.1f, 1000), window(mainWindow)
{
	//initialize fn to an lambda function with no body
	fn = []() {};

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
/*Call this to update the common global uniforms.
* this updates the Projection matrix and ViewMatrix which is common to all shaders
*/
void Scene::updateUniformBuffer(Camera& cam)
{
	glm::mat4 projMat = cam.getProjectionMatrix();
	glm::mat4 viewMat = cam.getViewMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrixBufferID);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, mat4Size, glm::value_ptr(projMat));
	glBufferSubData(GL_UNIFORM_BUFFER, mat4Size, mat4Size, glm::value_ptr(viewMat));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

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

	//register system and it's signature
	renderSys=ecs->RegisterSystem<RenderingSystem>();
	behaviourSys=ecs->RegisterSystem<BehaviourSystem>();
	physicsSys = ecs->RegisterSystem<physics::RigidBodySystem>();
	behaviourSys->ecs = ecs;
	physicsSys->ecs=ecs;
	ecs->SetSystemSignature<RenderingSystem>(renderSysSig);
	ecs->SetSystemSignature<BehaviourSystem>(behSysSig);
	ecs->SetSystemSignature<physics::RigidBodySystem>(physicsSysSig);


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

	glEnable(GL_DEPTH_TEST);

	//renderStuff
		updateUniformBuffer(cam);
		renderSys->Run(ecs,cam);
		behaviourSys->Update(deltaTime);
		physicsSys->Run(deltaTime);
		fn();


	window.processInput();
	window.swapBuffers();
	glfwPollEvents();
	deltaTime = currTime - lastTime;
	lastTime = currTime;
}
