#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>

#include<vector>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include"snakeBehaviour.h"
#include"Core/Editor/EditorUI.h"
#define sizeX 2.0f
#define sizeY 2.0f


int main()
{
	AssetManager::setAssetRoot("../../../Assets/");
	std::string rootDir(AssetManager::getAssetRoot());
	Window window(800, 800, "SnakeGame");
	
	if (!window.initialize())
	{
		std::cout << "Can't init WINDOW";
		return -1;
	}

	Scene scene(window);
	
	Material floorMat("DEFERRED");
	floorMat.setTexture2D("material.diffuseMap", rootDir + "Media/pbr/crate/basecolor.jpg");
	floorMat.setTexture2D("material.specularMap", rootDir + "Media/pbr/crate/roughness.jpg");
	floorMat.setTexture2D("material.normalMap", rootDir + "Media/pbr/crate/normal.jpg");
	floorMat.setValue("normalStrength", 0.1f);
	floorMat.setValue("specIntensity", 1.0f);

	EntityID dir = scene.CreateEntity();

	Lights &d = scene.AddComponent<Lights>(dir)=Lights(LIGHT::DIRECTIONAL);
	d.setColor(1, 1, 0.9);
	d.setDirection(-1, -1, -1);
	d.setIntensity(1);

	scene.AddComponent<Transform>(dir)= Transform(0, 2.0f, 0);


	
	EntityID snake = scene.CreateEntity();
	scene.AddComponent<BehaviourComponent>(snake);
	scene.GetComponent<BehaviourComponent>(snake).setBehaviour<Snake>(scene.cam,1,sizeX,sizeY);


	EntityID floor = scene.CreateEntity();
	Transform T(0, -(sizeY+0.5f), 0);
	T.SetGlobalScale(glm::vec3(sizeY*2+0.01f));
	scene.AddComponent<Transform>(floor)=T;
	Mesh &fMesh= scene.AddComponent<Mesh>(floor);
	fMesh.CreateMesh(generateCubeVertices());
	scene.AddComponent<Material>(floor)=(floorMat);
	//scene.vsyncOn(true);
	Editor editor(window,scene);
	
	scene.OnStart();
	while (!window.closeWindow())
	{
		scene.clearBuffer();
	
		scene.Render();
		editor.DrawUI();
		scene.swapBuffers();
		
	}



	return 0;
}





