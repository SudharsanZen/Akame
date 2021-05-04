#include "Editor/Scene.h"

void Scene::updateUniformBuffer(Camera& cam)
{
	glm::mat4 projMat = cam.getProjectionMatrix();
	glm::mat4 viewMat = cam.getViewMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrixBufferID);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, mat4Size, glm::value_ptr(projMat));
	glBufferSubData(GL_UNIFORM_BUFFER, mat4Size, mat4Size, glm::value_ptr(viewMat));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

void Scene::flyCam(Camera& cam, Window& window)
{
	static bool init = false;
	static double xPrev = 0, yPrev = 0;
	double x = 0, y = 0;
	static float X = -45, Y = -135;
	float speed = 10;
	if (Input::getKeyDown(GLFW_KEY_LEFT_SHIFT))
	{
		speed *= 5;
	}
	if (Input::getKeyDown(GLFW_KEY_W))
	{
		cam.setCameraPosition(cam.getCameraPosition() + cam.transform.forward() * speed * deltaTime);

	}
	if (Input::getKeyDown(GLFW_KEY_S))
	{
		cam.setCameraPosition(cam.getCameraPosition() - cam.transform.forward() * speed * deltaTime);

	}
	if (Input::getKeyDown(GLFW_KEY_A))
	{
		cam.setCameraPosition(cam.getCameraPosition() - cam.transform.right() * speed * deltaTime);

	}
	if (Input::getKeyDown(GLFW_KEY_D))
	{
		cam.setCameraPosition(cam.getCameraPosition() + cam.transform.right() * speed * deltaTime);

	}
	if (init)
		Input::getMouseXY(x, y);

	if (Input::getMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS || !init)
	{
		init = true;
		X += -(y - yPrev) * 0.5;
		Y += -(x - xPrev) * 0.5;
		//if (abs(Y) >= 360.0f)
			//Y =Y+((Y>0)?-1:1)*360.0f;
		Y = abs(Y) >= 360.0f ? 0 : Y;
		X = abs(X) >= 360.0f ? 0 : X;
		cam.transform.rotation = Quaternion::rotationAroundAxisVector(Y, worldUp);
		cam.transform.rotation = Quaternion::rotationAroundAxisVector(X, cam.transform.right()) * cam.transform.rotation.quaternion;
	}

	updateUniformBuffer(cam);
	cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
	yPrev = y;
	xPrev = x;
}

void Scene::InitEcs()
{
	ecs = std::make_shared<ECS>();

	//register components
	ecs->RegisterComponent<Mesh>();
	ecs->RegisterComponent<Transform>();
	ecs->RegisterComponent<Material>();
	ecs->RegisterComponent<BehaviourComponent>();



	//create system signature
	Signature renderSysSig;
	renderSysSig.set(ecs->GetComponentBitPose<Mesh>(),true);
	renderSysSig.set(ecs->GetComponentBitPose<Transform>(),true);
	renderSysSig.set(ecs->GetComponentBitPose<Material>(),true);

	Signature behSysSig;
	behSysSig.set(ecs->GetComponentBitPose<BehaviourComponent>(),true);

	//register system and it's signature
	renderSys=ecs->RegisterSystem<RenderingSystem>();
	behaviourSys=ecs->RegisterSystem<BehaviourSystem>();

	ecs->SetSystemSignature<RenderingSystem>(renderSysSig);
	ecs->SetSystemSignature<BehaviourSystem>(behSysSig);


}

void Scene::Render()
{
	float currTime = glfwGetTime();
	flyCam(cam, window);

	
	
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glClearColor(1,1,1,1.0f);
	glClearColor(color.x,color.y,color.z,color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	//renderStuff
		renderSys->Run(ecs,cam);
		behaviourSys->Update(ecs,deltaTime);
		fn();


	window.processInput();
	window.swapBuffers();
	glfwPollEvents();
	deltaTime = currTime - lastTime;
	lastTime = currTime;
}
