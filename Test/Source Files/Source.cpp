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
int main()
{

	std::string rootDir(AssetManager::getAssetRoot());
	Window window(800, 800, "testWindow");

	if (!window.initialize())
	{
		std::cout << "Can't init WINDOW";
		return -1;
	}
	Scene scene(window);


	EntityID dir = scene.CreateEntity();
	Lights d = Lights(LIGHT::DIRECTIONAL);
	d.setColor(1, 1, 1);
	d.setDirection(1, -1, 1);
	d.setIntensity(0.3f);
	d.setPointLightConst(1, 2, 10);
	scene.AddComponent<Lights>(dir, d);
	scene.AddComponent<Transform>(dir, Transform(0, 2, 0));

	EntityID ptl = scene.CreateEntity();
	Lights p = Lights(LIGHT::POINT);
	p.setColor(1, 1, 1);
	p.setIntensity(5);
	p.setPointLightConst(1, 1, 1);
	scene.AddComponent<Lights>(ptl, p);
	scene.AddComponent<Transform>(ptl, Transform(0,5,0));
	
	EntityID plane = scene.CreateEntity();
	
	Material spMat("DEFERRED");
	spMat.setTexture2D("material.diffuseMap", rootDir + "Media/pbr/bark/basecolor.jpg");
	spMat.setTexture2D("material.specularMap", rootDir + "Media/pbr/bark/roughness.jpg");
	spMat.setTexture2D("material.normalMap", rootDir + "Media/pbr/bark/normal.jpg");
	spMat.setValue("normalStrength", 1);
	spMat.setValue("specIntensity", 1);

	

	Material boxMat("DEFERRED");
	boxMat.setTexture2D("material.diffuseMap", rootDir + "Media/pbr/box/diffuse.png");
	boxMat.setTexture2D("material.specularMap", rootDir + "Media/pbr/box/roughness.png");
	boxMat.setTexture2D("material.normalMap", rootDir + "Media/pbr/normal.jpg");
	boxMat.setValue("normalStrength",0);
	boxMat.setValue("specIntensity", 1);


	Material planeMat("DEFERRED");
	planeMat.setTexture2D("material.diffuseMap", rootDir+"Media/pbr/rust/diffuse.png");
	planeMat.setTexture2D("material.specularMap", rootDir+"Media/pbr/rust/roughness.png");
	planeMat.setTexture2D("material.normalMap", rootDir + "Media/pbr/rust/normal.png");
	planeMat.setValue("normalStrength", 0.2f);
	planeMat.setValue("specIntensity", 1);

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
	planeShape.setColliderShape(physics::Shapes::PLANE, 20, 20);
	rbdy2.setRigidBodyType(physics::RigidBodyType::STATIC, planeShape);



	
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
			if (r)
			{
				scene.AddComponent<Material>(box, spMat);
				scene.GetComponent<Mesh>(box).CreateMesh(generateSphereVertices(32, 16, 1));
			}
			else
			{
				scene.AddComponent<Material>(box, boxMat);
				scene.GetComponent<Mesh>(box).CreateMesh(generateCubeVertices());
			}
			scene.AddComponent<Transform>(box, Transform(0, 3, 0));
			
			scene.GetComponent<Transform>(box).rotation.setEulerAngle(0, 0, 0);
			scene.AddComponent<physics::RigidBody3D>(box, physics::RigidBody3D());
			physics::RigidBody3D& rbdy = scene.GetComponent<physics::RigidBody3D>(box);
			
			if(r)
			rbdy.setRigidBodyType(physics::RigidBodyType::DYNAMIC, physics::Shapes::SPHERE);
			else
			rbdy.setRigidBodyType(physics::RigidBodyType::DYNAMIC, physics::Shapes::BOX);
		}
		acc +=scene.getDeltaTime();


		flyCam(scene.cam, scene.getDeltaTime());
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		scene.Render();


	}

	std::cout <<"\ncount:"<<count<<"\n";

	return 0;
}





