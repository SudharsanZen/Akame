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
#include"XMLReaderTest.h"

int app()
{
	std::string rootDir(AssetManager::getAssetRoot());
	Window window(800, 800, "testWindow");

	if (!window.initialize())
	{
		std::cout << "Can't init WINDOW";
		return -1;
	}
	Scene scene(window);



	EntityID plane = scene.CreateEntity();



	Material boxMat("DEFERRED");
	boxMat.setTexture2D("material.diffuseMap", rootDir + "Media/pbr/box/diffuse.png");
	boxMat.setTexture2D("material.specularMap", rootDir + "Media/pbr/box/roughness.png");



	Material planeMat("DEFERRED");
	planeMat.setTexture2D("material.diffuseMap", rootDir + "Media/pbr/rust/diffuse.png");
	planeMat.setTexture2D("material.specularMap", rootDir + "Media/pbr/rust/roughness.png");



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


	const int num = 256;
	const int rt = sqrt(num);
	for (int i = 0; i < num; i++)
	{
		EntityID box = scene.CreateEntity();
		scene.AddComponent<Mesh>(box, Mesh());
		scene.GetComponent<Mesh>(box).CreateMesh(generateCubeVertices());
		scene.AddComponent<Transform>(box, Transform(2 * (i / rt), 0, (2) * (i % rt)));
		scene.AddComponent<Material>(box, boxMat);
		scene.GetComponent<Transform>(box).rotation.setEulerAngle(0, 0, 0);
	}

	scene.OnStart();

	scene.backGroundColor(0, 0, 0, 1);
	float step = 0.3f;
	while (!window.closeWindow())
	{

		flyCam(scene.cam, scene.getDeltaTime());
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		scene.Render();

	}

	
}
int main()
{

	app();

	return 0;
}





