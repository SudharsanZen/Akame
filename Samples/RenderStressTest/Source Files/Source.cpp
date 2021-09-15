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

#include"Rendering/DeferredRendererFragmentBuffer.h"
#include"Core/Editor/EditorUI.h"
void app()
{
	AssetManager::setAssetRoot("../../../../Assets/");
	std::string rootDir(AssetManager::getAssetRoot());
	Window window(800, 800, "testWindow");
	std::cout<<sizeof(glm::mat4);
	if (!window.initialize())
	{
		std::cout << "Can't init WINDOW";

	}
	Scene scene(window);


	EntityID plane = scene.CreateEntity();
	EntityID plane2 = scene.CreateEntity();
	
	Material boxMat("DEFERRED");
	boxMat.setTexture2D("material.diffuseMap", rootDir + "Media/pbr/crate/basecolor.jpg");
	boxMat.setTexture2D("material.specularMap", rootDir + "Media/pbr/crate/roughness.jpg");
	boxMat.setTexture2D("material.normalMap",rootDir+"Media/pbr/crate/normal.jpg");
	boxMat.setValue("specIntensity", 1);
	boxMat.setValue("normalStrength",1);

	scene.vsyncOn(true);
	Material planeMat("DEFERRED");
	planeMat.setTexture2D("material.diffuseMap", rootDir + "Media/pbr/rust/diffuse.png");
	planeMat.setTexture2D("material.specularMap", rootDir + "Media/pbr/rust/roughness.png");
	planeMat.setTexture2D("material.normalMap", rootDir + "Media/pbr/rust/normal.png");
	planeMat.setValue("specIntensity",1);
	planeMat.setValue("normalStrength", 0.1f);


	Transform planeTransform;

	planeTransform.SetGlobalScale(glm::vec3(1) *= 50);
	planeTransform.SetGlobalPosition(glm::vec3(0.0f,0.1f,0.0f));
	Mesh &mP=scene.AddComponent<Mesh>(plane);
	mP.CreateMesh(generatePlaneVertices());
	scene.AddComponent<Transform>(plane)=planeTransform;
	scene.AddComponent<Material>(plane)=planeMat;

	physics::RigidBody3D& rbdy2=scene.AddComponent<physics::RigidBody3D>(plane);
	physics::ColliderShape planeShape;
	planeShape.setColliderShape(physics::Shapes::PLANE, 30, 30);
	rbdy2.setRigidBodyType(physics::RigidBodyType::STATIC, planeShape);

	Transform p2(0,5,0);
	p2.SetGlobalRotation(Quaternion(90,0,0));
	p2.SetGlobalScale(glm::vec3( 5));

	Mesh &plane2M=scene.AddComponent<Mesh>(plane2);
	plane2M.CreateMesh(generatePlaneVertices());
	scene.AddComponent<Transform>(plane2)=p2;
	scene.AddComponent<Material>(plane2)=planeMat;

	EntityID dir = scene.CreateEntity();
	Lights &d = scene.AddComponent<Lights>(dir);
	d.setType(LIGHT::DIRECTIONAL);
	d.setColor(1, 1, 1);
	d.setDirection(-1,-1,0);
	d.setIntensity(0.6f);
	d.setPointLightConst(1,2,10);
	d.ambientLigting(0.1f,0.1f,0.1f);
	
	scene.AddComponent<Transform>(dir)= Transform(0,2,0);

	Material mat("GRIDS");
	EntityID pl = scene.CreateEntity();
	Transform planeInf;

	scene.AddComponent<Transform>(pl)= planeInf;
	Mesh &plm=scene.AddComponent<Mesh>(pl);
	scene.AddComponent<Material>(pl)= mat;
	plm.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	Material matS("SPHERE");
	EntityID sky = scene.CreateEntity();
	Transform skyInf;
	

	scene.AddComponent<Transform>(sky) =skyInf;
	Mesh &skym=scene.AddComponent<Mesh>(sky);
	scene.AddComponent<Material>(sky)=matS;
	skym.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	
	const int num =200;
	const int rt = (int)sqrt(num);
	std::vector<EntityID> lightsVec;
	for (int i = 0; i < num; i++)
	{
		float off = (float)(rt-1);
	

		EntityID point = scene.CreateEntity();
		Lights &p = scene.AddComponent<Lights>(point);
	
		p.setColor(1,0.5,0);
		p.setIntensity(4);
		p.setPointLightConst(1, 2, 10);
		p.ambientLigting(0, 0, 0);
		
		scene.AddComponent<Transform>(point)= Transform(2 * (i / rt)-1 -off, 0.5, (2) * (i % rt)-off);
		
		lightsVec.push_back(point);
		
		EntityID box = scene.CreateEntity();
		Mesh& bm=scene.AddComponent<Mesh>(box);
		if ((i / rt) % 2 == 1)
		bm.CreateMesh(generateCubeVertices());
		if((i/rt)%2==0)
		bm.CreateMesh(generateSphereVertices(16,32,0.5));
		scene.AddComponent<Transform>(box)= Transform(2.0f * (i / rt)-off, 0.5f, (2.0f) * (i % rt)-off);
		scene.AddComponent<Material>(box)=boxMat;
		scene.GetComponent<Transform>(box).SetGlobalRotation(Quaternion(0, 0, 0));
	}
	Editor edt(window,scene);
	scene.OnStart();
	scene.vsyncOn(true);
	scene.backGroundColor(0, 0, 0, 1);
	float step = 0.3f;
	float acc = 0;
	while (!window.closeWindow())
	{
		acc += scene.getDeltaTime();
		
		flyCam(scene.cam, scene.getDeltaTime());
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
	
		scene.Render();
		edt.DrawUI();
		
		for (int i = 0; i < lightsVec.size(); i++)
		{
		
			Transform& T = scene.GetComponent<Transform>(lightsVec[i]);
			glm::vec3 pose = T.GetGlobalPosition();
			T.SetGlobalPosition(glm::vec3(pose.x, 1 + sin(acc + i),pose.z));
		}
		Lights &t=scene.GetComponent<Lights>(dir);
		Quaternion rot(0,0, fmod(acc * 10.0f, 180));
		//t.setDirection(rot*glm::vec3(-1,0, 0));
		scene.swapBuffers();
	}

	
}
int main()
{

	app();

	return 0;
}





