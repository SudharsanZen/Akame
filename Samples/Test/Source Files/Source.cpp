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


	EntityID dir = scene.CreateEntity();
	Lights &d = scene.AddComponent<Lights>(dir) = Lights(LIGHT::DIRECTIONAL);
	d.setColor(1, 1, 1);
	d.setDirection(1, -1, 1);
	d.setIntensity(0.3f);
	d.setPointLightConst(1, 2, 10);
	
	scene.AddComponent<Transform>(dir)= Transform(0, 2, 0);

	EntityID ptl = scene.CreateEntity();
	Lights &p = scene.AddComponent<Lights>(ptl)= Lights(LIGHT::POINT);
	p.setColor(1, 1, 1);
	p.setIntensity(5);
	p.setPointLightConst(1, 1, 1);
	
	scene.AddComponent<Transform>(ptl)= Transform(0,5,0);
	
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

	planeTransform.SetGlobalScale(glm::vec3( 20.0f));

	scene.AddComponent<Mesh>(plane).CreateMesh(generatePlaneVertices());
	scene.AddComponent<Transform>(plane)=planeTransform;
	scene.AddComponent<Material>(plane)=planeMat;

	physics::RigidBody3D& rbdy2 = scene.AddComponent<physics::RigidBody3D>(plane);
	physics::ColliderShape planeShape;
	planeShape.setColliderShape(physics::Shapes::PLANE, 20, 20);
	rbdy2.setRigidBodyType(physics::RigidBodyType::STATIC, planeShape);



	
	float acc = 0;
	scene.OnStart();
	unsigned int count = 0;
	scene.backGroundColor(0, 0, 0, 1);
	float step = 0.3f;
	Editor editor(window,scene);
	while (!window.closeWindow())
	{
		if (acc > step)
		{
			int r = rand() % 2;
			count++;
			acc -= step;
			EntityID box = scene.CreateEntity();
			
			if (r)
			{
				scene.AddComponent<Material>(box)= spMat;
				scene.AddComponent<Mesh>(box).CreateMesh(generateSphereVertices(32, 16, 1));
			}
			else
			{
				scene.AddComponent<Material>(box)=boxMat;
				scene.AddComponent<Mesh>(box).CreateMesh(generateCubeVertices());
			}
			scene.AddComponent<Transform>(box) =Transform(0, 3, 0);
			
		
			scene.AddComponent<physics::RigidBody3D>(box);
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
		editor.DrawUI();
		scene.swapBuffers();

	}

	std::cout <<"\ncount:"<<count<<"\n";

	return 0;
}





