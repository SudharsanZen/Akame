#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<vector>
#include<stdlib.h>
#include<Engine.h>
#include<DeferredRendererFragmentBuffer.h>
#include<Editor\Scene.h>
#define sizeX 10.0f
#define sizeY 10.0f

int main()
{
	
	std::string rootDir(AssetManager::getAssetRoot());
	Window window(800, 800, "DUMBSTUFF");
	
	if (!window.initialize())
	{
		std::cout << "Can't init WINDOW";
		return -1;
	}

	
	Scene scene(window);
	EntityID cube = scene.CreateEntity();
	Material cmat;
	cmat.setTexture2D("material.diffuseMap",AssetManager::assetRootPath+"Media/pbr/basecolor.jpg");
	cmat.setTexture2D("material.normalMap",AssetManager::assetRootPath+"Media/pbr/normal.jpg");
	cmat.setTexture2D("material.specularMap",AssetManager::assetRootPath+"Media/pbr/roughness.jpg");
	Transform cT(0,0,0);

	scene.AddComponent<Transform>(cube,cT);
	scene.AddComponent<Mesh>(cube, Mesh());
	scene.AddComponent<Material>(cube, cmat);
	scene.GetComponent<Mesh>(cube).CreateMesh(generateCubeVertices());

	Material mat("GRIDS");
	EntityID plane;
	Transform planeInf;
	planeInf.scale *= 1;
	scene.AddComponent<Transform>(plane,planeInf);
	scene.AddComponent<Mesh>(plane,Mesh());
	scene.AddComponent<Material>(plane,mat);
	scene.GetComponent<Mesh>(plane).CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	glm::vec3 point(0, 0, 100);
	point = scene.cam.getProjectionMatrix() * glm::vec4(point, 1);
	std::cout << "zpoint after trasnform:" << point.z << "\n\n\n";
	scene.OnStart();
	scene.vsyncOn(true);
	while (!window.closeWindow())
	{
		flyCam(scene.cam,scene.getDeltaTime());
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		scene.Render();
	}



	return 0;
}





