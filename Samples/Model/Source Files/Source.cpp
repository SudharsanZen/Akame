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
#include<Rendering/DeferredRendererFragmentBuffer.h>

class rotateBehv :public Behaviour
{
	float angle = 180.0f;
public:
	void OnStart() override
	{
	}
	void Update(float deltaTime) override
	{
		angle += deltaTime * 5.0f;
		angle = angle - floor(angle / 360.0f) * 360.0f;
		Transform& t = GetComponent<Transform>();
		t.SetGlobalRotation(Quaternion::rotationAroundAxisVector(angle,glm::vec3(0,1,0)));
	}
};

class strafe :public Behaviour
{
	float acc = 0;
	float offSet;
public:
	strafe(float off)
	{
		offSet = off;
	}
	void OnStart() override
	{
	}
	void Update(float deltaTime) override
	{
		acc += deltaTime;
		Transform& t = GetComponent<Transform>();
		glm::vec3 pose = t.GetGlobalPosition();
		t.SetGlobalPosition(glm::vec3(pose.x,pose.y, sin(offSet + acc) * 3.0f));
	}
};

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
	scene.cam.setFar(1000);

	EntityID dir = scene.CreateEntity();
	Lights d = Lights(LIGHT::DIRECTIONAL);
	d.setColor(1, 1, 1);
	d.setDirection(0, -1, 0);
	d.setIntensity(1);
	d.setPointLightConst(1, 2, 10);
	scene.AddComponent<Lights>(dir)=d;
	scene.AddComponent<Transform>(dir);
	
	EntityID model=LoadModelToScene(scene,"D:/Projects/GameEngine/sponza/sponza.fbx");
	//scene.AddComponent<BehaviourComponent>(bag).setBehaviour<rotateBehv>();
	Transform &T=scene.GetComponent<Transform>(model);
	T.SetGlobalScale(glm::vec3(0.5));
	T.SetGlobalPosition(glm::vec3(0,12,0));

	
	
	Material boxMat("DEFERRED");
	boxMat.setTexture2D("material.diffuseMap", rootDir + "Media/pbr/crate/basecolor.jpg");
	boxMat.setTexture2D("material.specularMap", rootDir + "Media/pbr/crate/roughness.jpg");
	boxMat.setTexture2D("material.normalMap", rootDir + "Media/pbr/crate/normal.jpg");
	boxMat.setValue("specIntensity", 1);
	boxMat.setValue("normalStrength", 1);
	EntityID box = scene.CreateEntity();
	Mesh& bm = scene.AddComponent<Mesh>(box);
	
		
	
		bm.CreateMesh(generateSphereVertices(16, 32, 0.5));
		Transform &t=	scene.AddComponent<Transform>(box);
		t =Transform(0, 10, 0);
	scene.AddComponent<Material>(box) = boxMat;
	scene.GetComponent<Transform>(box).SetGlobalRotation(Quaternion(0, 0, 0));

	std::vector<EntityID> lights;
	int m = 40;
	int maxi = 20;
	for (int i = 0; i < m; i++)
	{
		lights.push_back(scene.CreateEntity());
		Transform &t=scene.AddComponent<Transform>(lights[i]);
		t.SetGlobalPosition(glm::vec3((i-m/2)%(maxi/2),(i/maxi)*5.0f,0));
		Lights &l=scene.AddComponent<Lights>(lights[i]);

		l.setType(LIGHT::POINT);
		l.setPointLightConst(4,2,2);
		l.setIntensity(3);
		l.setColor(1,0.5,0);
		scene.AddComponent<BehaviourComponent>(lights[i]).setBehaviour<strafe>(float(i));
	}
	
	Editor e(window,scene);
	
	float acc = 0;
	scene.OnStart();
	scene.vsyncOn(false);
	unsigned int count = 0;
	scene.backGroundColor(0, 0, 0, 1);
	float step = 0.3f;
	while (!window.closeWindow())
	{
		
		t.SetGlobalPosition(glm::vec3(5,fabs(sin(acc))*30,0));
		acc += scene.getDeltaTime()/3;
		flyCam(scene.cam, scene.getDeltaTime());
		scene.clearBuffer();
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		scene.Render();
		e.DrawUI();

		Lights &l=scene.GetComponent<Lights>(dir);
	
		scene.swapBuffers();


	}

	std::cout <<"\ncount:"<<count<<"\n";

	return 0;
}





