
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
	T.scale = glm::vec3(4, 1.5, 4) * 5.0f;
	m->use(T, glm::vec3(10, 10, 10), scene->cam.getCameraPosition());
	mesh->renderMesh();

}

int main()
{
	AssetManager::setAssetRoot("../../../Assets/");
	std::string rootDir(AssetManager::getAssetRoot());
	Window window(800, 800, "GeoSpatial Data");

	if (!window.initialize())
		std::cout << "Something went wrong, can't initialize window";

	scene = std::make_shared<Scene>(window);

	mesh = std::make_shared<Mesh>();
	m = std::make_shared<DisplacementMap>();



	m->setDisplacementMap(rootDir + "Media/disp.jpg");
	m->setDiffuseMap(rootDir + "Media/diff.jpg");
	m->setSpecularMap(rootDir + "Media/disp.jpg");


	mesh->CreateMesh(generatePlaneVertices(500, 500));

	scene->backGroundColor(0, 0, 0, 1);
	scene->fn = func;
	while (!window.closeWindow())
	{
		flyCam(scene->cam, scene->getDeltaTime());
		scene->cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		scene->Render();
	}
	mesh.reset();
	m.reset();
	scene.reset();
	return 0;
}
