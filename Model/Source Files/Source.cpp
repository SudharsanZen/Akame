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


class rotateBehv :public Behaviour
{
	float angle = 180;
public:
	void OnStart() override
	{
	}
	void Update(float deltaTime) override
	{
		angle += deltaTime * 5;
		angle = angle - floor(angle / 360) * 360;
		Transform& t = GetComponent<Transform>();
		t.SetGlobalRotation(Quaternion::rotationAroundAxisVector(angle,glm::vec3(0,1,0)));
	}
};


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
	d.setIntensity(1);
	d.setPointLightConst(1, 2, 10);
	scene.AddComponent<Lights>(dir)=d;
	scene.AddComponent<Transform>(dir);

	Material spMat("DEFERRED");
	spMat.setTexture2D("material.diffuseMap", rootDir + "Media/backpack/diffuse.jpg");
	spMat.setTexture2D("material.specularMap", rootDir + "Media/backpack/roughness.jpg");
	spMat.setTexture2D("material.normalMap", rootDir + "Media/backpack/normal.png");
	spMat.setValue("normalStrength", 0.1f);
	spMat.setValue("specIntensity", 1);
	


	
	Model testModel(AssetManager::assetRootPath+"Media/backpack/backpack.obj");
	

	

	for (int i = 0; i < testModel.meshes.size(); i++)
	{
		Transform t(0, 12, 0);
		t.SetGlobalScale(t.GetGlobalScale()*7.0f);
		
		EntityID bag = scene.CreateEntity();
		scene.AddComponent<Mesh>(bag)= testModel.meshes[i];
		scene.AddComponent<Transform>(bag)=t;
		scene.AddComponent<Material>(bag)= spMat;
		scene.AddComponent<BehaviourComponent>(bag).setBehaviour<rotateBehv>();
	}



	
	float acc = 0;
	scene.OnStart();
	unsigned int count = 0;
	scene.backGroundColor(0, 0, 0, 1);
	float step = 0.3f;
	while (!window.closeWindow())
	{
		


		flyCam(scene.cam, scene.getDeltaTime());
		scene.clearBuffer();
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		scene.Render();
		scene.swapBuffers();


	}

	std::cout <<"\ncount:"<<count<<"\n";

	return 0;
}





