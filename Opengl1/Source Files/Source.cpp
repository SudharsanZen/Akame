#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include"Engine.h"
#include"Editor/Scene.h"
#include<vector>

class Rotate:public Behaviour
{
	float  y = 0;
	std::shared_ptr<Scene> scene;

public:
	Rotate(EntityID eid,std::shared_ptr<Scene> sc):Behaviour(eid)
	{
		scene = sc;
	}
	
	void OnStart()override
	{
	}
	void Update(float deltaTime)override
	{
		scene->GetComponent<Transform>(entityID).rotation.setEulerAngle(0,y+=deltaTime*10,0);
	}
};


int main()
{
	
	
	
	Window window(800,800,"Main Window",NULL);

	if (!window.initialize())
	{
		std::cout <<"ERROR::could not initialize Window!";
		return -1;
	}

	
	std::shared_ptr<Scene> scene=std::make_shared<Scene>(window);
	
	EntityID box = scene->CreateEntity();
	EntityID plane = scene->CreateEntity();
	
	Material m1;
	
	m1.setDiffuseMap("Assets/pbr/rust/diffuse.png");
	m1.setSpecularMap("Assets/pbr/rust/roughness.png");
	m1.setNormalMap("Assets/pbr/rust/normal.png");


	scene->AddComponent<Mesh>(box,Mesh());
	scene->AddComponent<Transform>(box,Transform(0,1,0));
	scene->AddComponent<Material>(box,m1);
	scene->AddComponent<BehaviourComponent>(box,BehaviourComponent());



	Transform planeT(0, 0, 0);
	planeT.scale*=10;
	scene->AddComponent<Mesh>(plane, Mesh());
	scene->AddComponent<Transform>(plane, planeT);
	scene->AddComponent<Material>(plane, m1);
	
	scene->GetComponent<Mesh>(box).CreateMesh(generateSphereVertices(10,10,1),std::vector<GLuint>());
	scene->GetComponent<Mesh>(plane).CreateMesh(generatePlaneVertices(10,10),std::vector<GLuint>());
	scene->GetComponent<BehaviourComponent>(box).setBehaviour<Rotate>(box,scene);


	
	

	while (!window.shouldWindowClose())
	{

		
		scene->Render();
	
	}


	return 0;
}





