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
	boxMat.setTexture2D("material.normalMap",rootDir+"Media/pbr/normal.jpg");
	boxMat.setValue("specIntensity", 2);
	boxMat.setValue("normalStrength",2);


	Material planeMat("DEFERRED");
	planeMat.setTexture2D("material.diffuseMap", rootDir + "Media/pbr/rust/diffuse.png");
	planeMat.setTexture2D("material.specularMap", rootDir + "Media/pbr/rust/roughness.png");
	planeMat.setTexture2D("material.normalMap", rootDir + "Media/pbr/rust/normal.png");
	planeMat.setValue("specIntensity",1);
	planeMat.setValue("normalStrength", 0.1f);


	Transform planeTransform;

	planeTransform.scale *= 20.0f;

	scene.AddComponent<Mesh>(plane, Mesh());
	scene.GetComponent<Mesh>(plane).CreateMesh(generatePlaneVertices());
	scene.AddComponent<Transform>(plane, planeTransform);
	scene.AddComponent<Material>(plane, planeMat);

	scene.AddComponent<physics::RigidBody3D>(plane, physics::RigidBody3D());
	physics::RigidBody3D& rbdy2 = scene.GetComponent<physics::RigidBody3D>(plane);
	physics::ColliderShape planeShape;
	planeShape.setColliderShape(physics::PLANE, 20, 20);
	rbdy2.setRigidBodyType(physics::STATIC, planeShape);



	EntityID dir = scene.CreateEntity();
	Lights d = Lights(LIGHT::DIRECTIONAL);
	d.setColor(1, 1, 1);
	d.setDirection(1,-1,1);
	d.setIntensity(1);
	d.setPointLightConst(1,2,10);
	scene.AddComponent<Lights>(dir, d);
	scene.AddComponent<Transform>(dir, Transform(0,2,0));

	
	const int num = 100;
	const int rt = sqrt(num);
	for (int i = 0; i < num; i++)
	{
		
		EntityID point = scene.CreateEntity();
		Lights p = Lights(LIGHT::POINT);

		p.setColor(1,0.5,0);
		p.setIntensity(4);
		p.setPointLightConst(1, 0.1, 5);
		scene.AddComponent<Lights>(point, p);
		scene.AddComponent<Transform>(point, Transform(2 * (i / rt), 1.1, (2) * (i % rt)));

		EntityID box = scene.CreateEntity();
		scene.AddComponent<Mesh>(box, Mesh());
		//if ((i / rt) % 2 == 1)
		scene.GetComponent<Mesh>(box).CreateMesh(generateCubeVertices());
		//if((i/rt)%2==0)
		//scene.GetComponent<Mesh>(box).CreateMesh(generateSphereVertices(10,10,0.5));
		scene.AddComponent<Transform>(box, Transform(2 * (i / rt), 0.5, (2) * (i % rt)));
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





