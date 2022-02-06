
#include"ExporterUI.h"

int main()
{
	
	

	
	AssetManager::assetRootPath = "../../../../Assets/";

	Window window(800,800,"Akame Exporter");
	if (!window.initialize())
	{
		throw("can't initialize window");
	}
	
	Scene scene(window);
	
	auto dir = scene.CreateEntity();
	scene.SetEntityName(dir,"Directional Light");
	scene.AddComponent<Transform>(dir);
	Lights d_light(LIGHT::DIRECTIONAL);
	d_light.setDirection(-90,0,0);
	scene.AddComponent<Lights>(dir) = d_light;



	Material mat("GRIDS");
	Entity pl = scene.CreateEntity();


	Transform &t=scene.AddComponent<Transform>(pl);
	Mesh& plm = scene.AddComponent<Mesh>(pl);
	scene.AddComponent<Material>(pl) = mat;
	plm.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);


	Exporter exp(scene);
	scene.vsyncOn(true);
	
	scene.backGroundColor(0,0,0,1);
	while (!window.closeWindow())
	{
		scene.clearBuffer();
			exp.DrawUI();
		scene.swapBuffers();
	}
	return 0;
}



