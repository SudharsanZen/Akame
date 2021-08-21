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
#include"DeferredRendererFragmentBuffer.h"
#include"Editor/EditorUI.h"
void app()
{
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

	planeTransform.scale *= 50;

	scene.AddComponent<Mesh>(plane, Mesh());
	scene.GetComponent<Mesh>(plane).CreateMesh(generatePlaneVertices());
	scene.AddComponent<Transform>(plane, planeTransform);
	scene.AddComponent<Material>(plane, planeMat);

	scene.AddComponent<physics::RigidBody3D>(plane, physics::RigidBody3D());
	physics::RigidBody3D& rbdy2 = scene.GetComponent<physics::RigidBody3D>(plane);
	physics::ColliderShape planeShape;
	planeShape.setColliderShape(physics::Shapes::PLANE, 30, 30);
	rbdy2.setRigidBodyType(physics::RigidBodyType::STATIC, planeShape);

	Transform p2(0,5,0);
	p2.rotation.setEulerAngle(90,0,0);
	p2.scale *= 5;

	scene.AddComponent<Mesh>(plane2, Mesh());
	scene.GetComponent<Mesh>(plane2).CreateMesh(generatePlaneVertices());
	scene.AddComponent<Transform>(plane2, p2);
	scene.AddComponent<Material>(plane2, planeMat);

	EntityID dir = scene.CreateEntity();
	Lights d = Lights(LIGHT::DIRECTIONAL);
	d.setColor(1, 1, 1);
	d.setDirection(0,-1,0);
	d.setIntensity(0.6f);
	d.setPointLightConst(1,2,10);
	d.ambientLigting(0.1f,0.1f,0.1f);
	scene.AddComponent<Lights>(dir, d);
	scene.AddComponent<Transform>(dir, Transform(0,2,0));

	Material mat("GRIDS");
	EntityID pl = scene.CreateEntity();
	Transform planeInf;
	planeInf.scale *= 1;
	scene.AddComponent<Transform>(pl, planeInf);
	scene.AddComponent<Mesh>(pl, Mesh());
	scene.AddComponent<Material>(pl, mat);
	scene.GetComponent<Mesh>(pl).CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	Material matS("SPHERE");
	EntityID sky = scene.CreateEntity();
	Transform skyInf;
	

	scene.AddComponent<Transform>(sky, skyInf);
	scene.AddComponent<Mesh>(sky, Mesh());
	scene.AddComponent<Material>(sky, matS);
	scene.GetComponent<Mesh>(sky).CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	
	const int num =800;
	const int rt = (int)sqrt(num);
	std::vector<EntityID> lightsVec;
	for (int i = 0; i < num; i++)
	{
		float off = (float)(rt-1);
	

		EntityID point = scene.CreateEntity();
		Lights p = Lights(LIGHT::POINT);
	
		p.setColor(1,0.5,0);
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
		scene.AddComponent<Transform>(box, Transform(2.0f * (i / rt)-off, 0.5f, (2.0f) * (i % rt)-off));
		scene.AddComponent<Material>(box, boxMat);
		scene.GetComponent<Transform>(box).rotation.setEulerAngle(0, 0, 0);
	}
	
	scene.OnStart();
	scene.vsyncOn(false);
	scene.backGroundColor(0, 0, 0, 1);
	float step = 0.3f;
	float acc = 0;
	while (!window.closeWindow())
	{
		acc += scene.getDeltaTime();
		flyCam(scene.cam, scene.getDeltaTime());
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());

		scene.Render();
		
		
		for (int i = 0; i < lightsVec.size(); i++)
		{
			scene.GetComponent<Transform>(lightsVec[i]).position.y=1+sin(acc+i);
		}
		Lights &t=scene.GetComponent<Lights>(dir);
		t.setDirection(glm::vec3(0, -1, 0));
		
	}

	
}
int main()
{

	app();

	return 0;
}





