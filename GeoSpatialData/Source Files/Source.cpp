#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include"Engine.h"
#include"DisplacementMap.h"
#include"Editor/Scene.h"
#include<vector>


std::shared_ptr<Mesh> mesh;
std::shared_ptr<DisplacementMap> m;
std::shared_ptr<Scene> scene;
void  func()
{
	static Transform T;
	T.scale=glm::vec3(4,1.5,4)*5.0f;
	m->use(T,glm::vec3(10,10,10),scene->cam.getCameraPosition());
	mesh->renderMesh();
	
}

int main()
{

	Window window(800, 800, "GeoSpatial Data");

	if (!window.initialize())
		std::cout << "Something went wrong, can't initialize window";

	scene= std::make_shared<Scene>(window);

	mesh = std::make_shared<Mesh>();
	m= std::make_shared<DisplacementMap>();
	
	

	m->setDisplacementMap("Assets/disp.jpg");
	m->setDiffuseMap("Assets/diff.jpg");
	m->setSpecularMap("Assets/disp.jpg");

	
	mesh->CreateMesh(generatePlaneVertices(200,200));

	scene->backGroundColor(0, 0, 0, 1);
	scene->fn = func;
	while (!window.closeWindow())
	{
		scene->Render();
	}

	return 0;
}






