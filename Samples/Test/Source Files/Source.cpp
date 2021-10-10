#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include"Core/Engine.h"
#include"Core/Scene.h"
#include<vector>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main()
{
	AssetManager::setAssetRoot("../../../../Assets/");
	std::string rootDir(AssetManager::getAssetRoot());
	Window window(800, 800, "testWindow");

	if (!window.initialize())
	{
		std::cout << "Can't init WINDOW";
		return -1;
	}
	Scene scene(window);


	Entity dir = scene.CreateEntity();
	Lights &d = scene.AddComponent<Lights>(dir) = Lights(LIGHT::DIRECTIONAL);
	d.setColor(1, 1, 1);
	d.setDirection(1, -1, 1);
	d.setIntensity(1);
	d.setPointLightConst(1, 2, 10);
	scene.SetEntityName(dir,"Directional light");
	scene.AddComponent<Transform>(dir).SetGlobalPosition(glm::vec3(0, 2, 0));

	Entity ptl = scene.CreateEntity();
	Lights &p = scene.AddComponent<Lights>(ptl)= Lights(LIGHT::POINT);
	p.setColor(1, 1, 1);
	p.setIntensity(5);
	p.setPointLightConst(1, 1, 1);
	scene.SetEntityName(ptl, "point light");
	scene.AddComponent<Transform>(ptl).SetGlobalPosition(glm::vec3(0,5,0));
	
	Entity plane = scene.CreateEntity();
	

	Material spMat("DEFERRED");
	spMat.setTexture2D("material.diffuse", rootDir + "Media/pbr/rust1/diffuse.png");
	spMat.setTexture2D("material.roughness", rootDir + "Media/pbr/rust1/roughness.png");
	spMat.setTexture2D("material.normal", rootDir + "Media/pbr/rust1/normal.png");
	spMat.setTexture2D("material.metallic", rootDir + "Media/pbr/rust1/metallic.png");
	spMat.setValue("noAO", 1);
	spMat.setValue("ambientocclusion", 1);
	spMat.setValue("noMetallic", 0);
	spMat.setValue("normalStrength", 1);

	


	Material boxMat("DEFERRED");
	boxMat.setTexture2D("material.diffuse", rootDir + "Media/pbr/crate/basecolor.jpg");
	boxMat.setTexture2D("material.roughness", rootDir + "Media/pbr/crate/roughness.jpg");
	boxMat.setTexture2D("material.normal", rootDir + "Media/pbr/crate/normal.jpg");
	boxMat.setTexture2D("material.ambientocclusion", rootDir + "Media/pbr/crate/ambientocclusion.jpg");
	boxMat.setValue("noAO", 0);
	boxMat.setValue("noMetallic", 1);
	boxMat.setValue("metallic", 0.04f);
	boxMat.setValue("normalStrength", 1);

	Material planeMat("DEFERRED");
	planeMat.setTexture2D("material.diffuse", rootDir + "Media/pbr/rust/diffuse.png");
	planeMat.setTexture2D("material.roughness", rootDir + "Media/pbr/rust/roughness.png");
	planeMat.setTexture2D("material.normal", rootDir + "Media/pbr/rust/normal.png");
	planeMat.setTexture2D("material.metallic", rootDir + "Media/pbr/rust/metallic.png");
	planeMat.setValue("noAO", 1);
	planeMat.setValue("ambientocclusion", 1);
	planeMat.setValue("noMetallic", 0);
	planeMat.setValue("normalStrength", 1);







	scene.AddComponent<Mesh>(plane).CreateMesh(generatePlaneVertices());
	scene.AddComponent<Transform>(plane).SetGlobalScale(glm::vec3(20.0f));
	scene.AddComponent<Material>(plane)=planeMat;

	physics::RigidBody3D& rbdy2 = scene.AddComponent<physics::RigidBody3D>(plane);
	physics::ColliderShape planeShape;
	planeShape.setColliderShape(physics::Shapes::PLANE, 20, 20);
	rbdy2.setRigidBodyType(physics::RigidBodyType::STATIC, planeShape);



	scene.vsyncOn(true);
	float acc = 0;
	scene.OnStart();
	unsigned int count = 0;
	scene.backGroundColor(0, 0, 0, 1);
	float step = 0.5f;
	Editor editor(window,scene);
	while (!window.closeWindow())
	{
		if (acc > step)
		{
			int r = rand() % 2;
			count++;
			acc -= step;
			Entity box = scene.CreateEntity();
			
			if (r)
			{
				scene.AddComponent<Material>(box)= spMat;
				scene.SetEntityName(box, "sphere (copy)");
				scene.AddComponent<Mesh>(box).CreateMesh(generateSphereVertices(32, 32, 1));
			}
			else
			{
				scene.AddComponent<Material>(box)=boxMat;
				scene.AddComponent<Mesh>(box).CreateMesh(generateCubeVertices());
				scene.SetEntityName(box, "cube (copy)");
			}
			scene.AddComponent<Transform>(box).SetGlobalPosition(glm::vec3(0, 3, 0));
			
		
			scene.AddComponent<physics::RigidBody3D>(box);
			physics::RigidBody3D& rbdy = scene.GetComponent<physics::RigidBody3D>(box);
			
			if(r)
			rbdy.setRigidBodyType(physics::RigidBodyType::DYNAMIC, physics::Shapes::SPHERE);
			else
			rbdy.setRigidBodyType(physics::RigidBodyType::DYNAMIC, physics::Shapes::BOX);
		}
		acc +=scene.getDeltaTime();


		flyCam(scene.cam, scene.getDeltaTime());
		scene.clearBuffer();
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		scene.Render();
		editor.DrawUI();
		scene.swapBuffers();

	}

	std::cout <<"\ncount:"<<count<<"\n";

	return 0;
}





