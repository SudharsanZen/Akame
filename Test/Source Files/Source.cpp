#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include"Engine.h"
#include"DisplacementMap.h"
#include"Editor/Scene.h"
#include<vector>



int main()
{


	std::shared_ptr<Shader>	shader;
	Window window(800, 800, "Test");

	if (!window.initialize())
		std::cout << "Something went wrong, can't initialize window";

	std::shared_ptr<Scene>	scene = std::make_shared<Scene>(window);

	int num = 2000;
	int sq = sqrt(num);
	Material m;
	
	m.setDiffuseMap("Assets/diffuse.png");
	m.setSpecularMap("Assets/roughness.png");
	
	for (int i = 0; i < num; i++)
	{
		EntityID eid = scene->CreateEntity();
		scene->AddComponent<Transform>(eid, Transform((i / sq) , 0, (i % sq)));
		scene->AddComponent<Mesh>(eid,Mesh());
		scene->AddComponent<Material>(eid,m);

		scene->GetComponent<Mesh>(eid).CreateMesh(generateCubeVertices());
	
	}
	m.setUniforms(glm::vec3(10, 10, 10), scene->cam.getCameraPosition());
	
	
	
	while (!window.closeWindow())
	{
		
		
		scene->Render();
	}

	return 0;
}