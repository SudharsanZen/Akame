
#include"ExporterUI.h"
/*A command line tool for converting well known 3d file formats to engine format*/
int main()
{
	AssetManager::assetRootPath = "../../../../Assets/";
	//need to write code lol
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


	scene.AddComponent<Transform>(pl);
	Mesh& plm = scene.AddComponent<Mesh>(pl);
	scene.AddComponent<Material>(pl) = mat;
	plm.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);


	Exporter exp(scene);
	scene.vsyncOn(true);
	//Editor edt(scene);
	scene.backGroundColor(0,0,0,1);
	while (!window.closeWindow())
	{
		scene.clearBuffer();
			
			//flyCam(scene.cam,scene.getDeltaTime());
			//scene.Render();
			exp.DrawUI();
		
		scene.swapBuffers();
	}
	return 0;
}





