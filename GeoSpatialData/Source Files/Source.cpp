
#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include"Core/Engine.h"
#include"Rendering/DisplacementMap.h"
#include"Core/Scene.h"
#include<vector>





int main()
{
	AssetManager::setAssetRoot("../../../Assets/");
	Window window(800, 800, "GeoSpatial Data");

	if (!window.initialize())
		std::cout << "Something went wrong, can't initialize window";
	Scene scene(window);
	
	Mesh mesh;
	DisplacementMap m;
	
	
	std::string rootDir(AssetManager::getAssetRoot());
	

	m.setDisplacementMap(rootDir + "Media/disp.jpg");
	m.setDiffuseMap(rootDir + "Media/diff.jpg");
	m.setSpecularMap(rootDir + "Media/disp.jpg");


	mesh.CreateMesh(generatePlaneVertices(500, 500));

	scene.backGroundColor(0, 0, 0, 1);

	while (!window.closeWindow())
	{
		flyCam(scene.cam, scene.getDeltaTime());
		scene.clearBuffer();
			scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
			scene.Render();
				static Transform T;
				T.SetGlobalScale(glm::vec3(4, 1.5, 4) * 5.0f);
				m.use(T, glm::vec3(10, 10, 10), scene.cam.getCameraPosition());
				mesh.renderMesh();
		scene.swapBuffers();
	}
	
	return 0;
}
