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
#include"Editor/EditorUI.h"
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
	boxMat.setTexture2D("material.diffuseMap", rootDir + "Media/pbr/crate/basecolor.jpg");
	boxMat.setTexture2D("material.specularMap", rootDir + "Media/pbr/crate/roughness.jpg");
	boxMat.setTexture2D("material.normalMap",rootDir+"Media/pbr/crate/normal.jpg");
	boxMat.setValue("specIntensity", 1);
	boxMat.setValue("normalStrength",1);


	Material planeMat("DEFERRED");
	planeMat.setTexture2D("material.diffuseMap", rootDir + "Media/pbr/rust/diffuse.png");
	planeMat.setTexture2D("material.specularMap", rootDir + "Media/pbr/rust/roughness.png");
	planeMat.setTexture2D("material.normalMap", rootDir + "Media/pbr/rust/normal.png");
	planeMat.setValue("specIntensity",1);
	planeMat.setValue("normalStrength", 0.1f);


	Transform planeTransform;

	planeTransform.scale *= 50;

	scene.AddComponent<Mesh>(plane, Mesh());
	scene.GetComponent<Mesh>(plane).CreateMesh(generatePlaneVertices());
	scene.AddComponent<Transform>(plane, planeTransform);
	scene.AddComponent<Material>(plane, planeMat);

	scene.AddComponent<physics::RigidBody3D>(plane, physics::RigidBody3D());
	physics::RigidBody3D& rbdy2 = scene.GetComponent<physics::RigidBody3D>(plane);
	physics::ColliderShape planeShape;
	planeShape.setColliderShape(physics::PLANE, 30, 30);
	rbdy2.setRigidBodyType(physics::STATIC, planeShape);



	EntityID dir = scene.CreateEntity();
	Lights d = Lights(LIGHT::DIRECTIONAL);
	d.setColor(1, 1, 1);
	d.setDirection(1,-1,1);
	d.setIntensity(0.6);
	d.setPointLightConst(1,2,10);
	d.ambientLigting(0.1,0,0);
	scene.AddComponent<Lights>(dir, d);
	scene.AddComponent<Transform>(dir, Transform(0,2,0));
	
	
	const int num =800;
	const int rt = sqrt(num);
	std::vector<EntityID> lightsVec;
	for (int i = 0; i < num; i++)
	{
		float off = rt-1;
	

		EntityID point = scene.CreateEntity();
		Lights p = Lights(LIGHT::POINT);
	
		p.setColor(1,1,1);
		p.setIntensity(4);
		p.setPointLightConst(1, 2, 10);
		p.ambientLigting(0, 0, 0);
		scene.AddComponent<Lights>(point, p);
		scene.AddComponent<Transform>(point, Transform(2 * (i / rt)-1 -off, 0.5, (2) * (i % rt)-off));
		
		lightsVec.push_back(point);
		
		EntityID box = scene.CreateEntity();
		scene.AddComponent<Mesh>(box, Mesh());
		if ((i / rt) % 2 == 1)
		scene.GetComponent<Mesh>(box).CreateMesh(generateCubeVertices());
		if((i/rt)%2==0)
		scene.GetComponent<Mesh>(box).CreateMesh(generateSphereVertices(16,32,0.5));
		scene.AddComponent<Transform>(box, Transform(2 * (i / rt)-off, 0.5, (2) * (i % rt)-off));
		scene.AddComponent<Material>(box, boxMat);
		scene.GetComponent<Transform>(box).rotation.setEulerAngle(0, 0, 0);
	}
	
	scene.OnStart();

	scene.backGroundColor(0, 0, 0, 1);
	float step = 0.3f;
	float acc = 0;
	while (!window.closeWindow())
	{
		acc += scene.getDeltaTime();
		flyCam(scene.cam, scene.getDeltaTime());
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());

		scene.Render();
		
		
		for (int i = 0; i < num; i++)
		{
			scene.GetComponent<Transform>(lightsVec[i]).position.y=1+sin(acc+i);
		}
	}

	
}
int main()
{

	app();

	return 0;
}





