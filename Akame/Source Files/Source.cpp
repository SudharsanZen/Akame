#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<vector>
#include<stdlib.h>
#include<Core/Engine.h>
#include<Rendering/DeferredRendererFragmentBuffer.h>
#include<Core/Scene.h>
#include<sstream>
#include"Core/Debug/Debug.h"
#include"Core/Editor/EditorUI.h"
#define sizeX 10.0f
#define sizeY 10.0f

int main()
{
	AssetManager::setAssetRoot("../../../Assets/");
	std::string rootDir(AssetManager::getAssetRoot());
	Window window(800, 800, "DUMBSTUFF");
	
	if (!window.initialize())
	{
		std::cout << "Can't init WINDOW";
		return -1;
	}

	

	Scene scene(window);
	Entity dir = scene.CreateEntity();
	Lights &d = scene.AddComponent<Lights>(dir);
	d.setType(LIGHT::DIRECTIONAL);
	d.setColor(1, 1, 1);
	d.setDirection(1,-1,0);
	d.setIntensity(0.6f);
	d.setPointLightConst(1,2,10);
	d.ambientLigting(0.1f,0.1f,0.1f);
	
	scene.AddComponent<Transform>(dir).SetGlobalPosition(glm::vec3(0,2,0));
	scene.SetEntityName(dir, "DirectionalLight");
	std::vector<Entity> cubeList;
	float m = 1;
	
		std::shared_ptr<Material> cmat = std::make_shared<Material>("DEFAULT");
		cmat->setTexture2D("material.diffuseMap", AssetManager::assetRootPath + "Media/pbr/basecolor.jpg");
		cmat->setTexture2D("material.normalMap", AssetManager::assetRootPath + "Media/pbr/normal.jpg");
		cmat->setTexture2D("material.specularMap", AssetManager::assetRootPath + "Media/pbr/roughness.jpg");
		cmat->setValue("normalStrength",10);
		


		     Entity cube = LoadModelToScene(scene,AssetManager::assetRootPath+"/media/pbr/box.obj");
			cubeList.push_back(cube);
			Transform& t = scene.AddComponent<Transform>(cube);
			t.SetGlobalRotation(Quaternion(0, 0, 0));
			t.SetGlobalPosition(glm::vec3(0, 0, 0));
			t.SetGlobalScale(glm::vec3(5));
			scene.AddComponent<Material>(cube) = *cmat;
			Mesh& cM = scene.AddComponent<Mesh>(cube);

			//cM.CreateMesh(generateSphereVertices(32, 16, 0.5));
			cM.CreateMesh(generateCubeVertices(2,2));


	

	Material mat("GRIDS");
	Entity plane= scene.CreateEntity();


	scene.AddComponent<Transform>(plane);
	Mesh &pm=scene.AddComponent<Mesh>(plane);
	scene.AddComponent<Material>(plane)=mat;
	pm.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	scene.SetEntityName(plane,"grids");
	
	Material matS("SPHERE");
	Entity sky= scene.CreateEntity();
	scene.AddComponent<Material>(sky) = matS;
	scene.AddComponent<Transform>(sky);
	Mesh& skMesh = scene.AddComponent<Mesh>(sky);
	skMesh.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	scene.SetEntityName(sky, "skyquad");
	
	Transform& tr = scene.GetComponent<Transform>(cubeList[0]);

	
	//t.SetGlobalRotation(Quaternion::rotationAroundAxisVector(30, glm::vec3(0, 0, 1)));

	Editor editor(window,scene);
	scene.OnStart();
	scene.vsyncOn(true);
	float deltaTime = 0;

	while (!window.closeWindow())
	{
		
		deltaTime = scene.getDeltaTime();
		flyCam(scene.cam,scene.getDeltaTime());
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		scene.clearBuffer();
	
		scene.Render();
		editor.DrawUI();
		
		glm::vec3 pose = tr.GetLocalPosition();

		tr.SetGlobalRotation(Quaternion::rotationAroundAxisVector(deltaTime*10,glm::vec3(0,1,0))* t.GetGlobalRotation());

		scene.swapBuffers();
		
		
	}



	return 0;
}





