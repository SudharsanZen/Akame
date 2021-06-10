#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include"Engine.h"
#include"Editor/Scene.h"
#include<vector>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>



int main()
{
	std::string rootDir(ASSETS_ROOT_DIR);
	Window window(800, 800, "testWindow");

	if (!window.initialize())
	{
		std::cout << "Can't init WINDOW";
		return -1;
	}
	Scene scene(window);



	EntityID plane = scene.CreateEntity();
	


	Material boxMat;
	boxMat.setDiffuseMap(rootDir+"Media/pbr/box/diffuse.png");
	boxMat.setSpecularMap(rootDir+"Media/pbr/box/roughness.png");

	Material planeMat;
	planeMat.setDiffuseMap(rootDir+"Media/pbr/rust/diffuse.png");
	planeMat.setSpecularMap(rootDir+"Media/pbr/rust/roughness.png");

	

	Transform planeTransform;

	planeTransform.scale *= 20.0f;

	scene.AddComponent<Mesh>(plane, Mesh());
	scene.GetComponent<Mesh>(plane).CreateMesh(generatePlaneVertices());
	scene.AddComponent<Transform>(plane, planeTransform);
	scene.AddComponent<Material>(plane, planeMat);
	//scene.GetComponent<Transform>(plane).rotation.setEulerAngle(0,0,45);
	scene.AddComponent<physics::RigidBody3D>(plane, physics::RigidBody3D());
	physics::RigidBody3D& rbdy2 = scene.GetComponent<physics::RigidBody3D>(plane);
	physics::ColliderShape planeShape;
	planeShape.setColliderShape(physics::PLANE, 20, 20);
	rbdy2.setRigidBodyType(physics::STATIC, planeShape);



	
	float acc = 0;
	scene.OnStart();
	unsigned int count = 0;
	scene.backGroundColor(0, 0, 0, 1);
	float step = 0.3f;
	while (!window.closeWindow())
	{
		if (acc > step)
		{
			int r = rand() % 2;
			count++;
			acc -= step;
			EntityID box = scene.CreateEntity();
			scene.AddComponent<Mesh>(box, Mesh());
			if(r)
			scene.GetComponent<Mesh>(box).CreateMesh(generateSphereVertices(32,16,1));
			else
			scene.GetComponent<Mesh>(box).CreateMesh(generateCubeVertices());
			scene.AddComponent<Transform>(box, Transform(0, 3, 0));
			scene.AddComponent<Material>(box, boxMat);
			scene.GetComponent<Transform>(box).rotation.setEulerAngle(0, 0, 0);
			scene.AddComponent<physics::RigidBody3D>(box, physics::RigidBody3D());
			physics::RigidBody3D& rbdy = scene.GetComponent<physics::RigidBody3D>(box);
			
			if(r)
			rbdy.setRigidBodyType(physics::DYNAMIC, physics::SPHERE);
			else
			rbdy.setRigidBodyType(physics::DYNAMIC, physics::BOX);
		}
		acc +=scene.getDeltaTime();


		flyCam(scene.cam, scene.getDeltaTime());
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		scene.Render();


	}

	std::cout <<"\ncount:"<<count<<"\n";

	return 0;
}





