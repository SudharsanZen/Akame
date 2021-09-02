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

	Material cmat("DEFERRED");
	cmat.setTexture2D("material.diffuseMap", AssetManager::assetRootPath + "Media/pbr/basecolor.jpg");
	cmat.setTexture2D("material.normalMap", AssetManager::assetRootPath + "Media/pbr/normal.jpg");
	cmat.setTexture2D("material.specularMap", AssetManager::assetRootPath + "Media/pbr/roughness.jpg");

	Scene scene(window);
	EntityID dir = scene.CreateEntity();
	Lights d = Lights(LIGHT::DIRECTIONAL);
	d.setColor(1, 1, 1);
	d.setDirection(1,-1,0);
	d.setIntensity(0.6f);
	d.setPointLightConst(1,2,10);
	d.ambientLigting(0.1f,0.1f,0.1f);
	scene.AddComponent<Lights>(dir, d);
	scene.AddComponent<Transform>(dir, Transform(0,2,0));
	std::vector<EntityID> cubeList;
	float m = 100;
	for (int i = 0; i < m; i++)
	{
		

		EntityID cube = scene.CreateEntity();
		cubeList.push_back(cube);
		
		Transform cT(float(i)*0.9f, 0, 0);
		scene.AddComponent<Transform>(cube, cT);
		scene.AddComponent<Mesh>(cube, Mesh());
		scene.AddComponent<Material>(cube, cmat);
		scene.GetComponent<Mesh>(cube).CreateMesh(generateSphereVertices(32,16,1));
	}
	

	

	Material mat("GRIDS");
	EntityID plane= scene.CreateEntity();
	Transform planeInf;
	planeInf.scale *= 1;
	scene.AddComponent<Transform>(plane,planeInf);
	scene.AddComponent<Mesh>(plane,Mesh());
	scene.AddComponent<Material>(plane,mat);
	scene.GetComponent<Mesh>(plane).CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	
	
	Material matS("SPHERE");
	EntityID sky= scene.CreateEntity();
	Transform skyInf;

	scene.AddComponent<Transform>(sky, skyInf);
	scene.AddComponent<Mesh>(sky, Mesh());
	scene.AddComponent<Material>(sky, matS);
	scene.GetComponent<Mesh>(sky).CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	
	
	scene.OnStart();
	scene.vsyncOn(false);
	float time = 0;
	while (!window.closeWindow())
	{
		time += scene.getDeltaTime();
		flyCam(scene.cam,scene.getDeltaTime());
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		scene.Render();

		for (int i = 0; i < m; i++)
		{
			float fixSt = (1 - ((m - i) / m)) * (20.0f / (i + 1));
			float fixEnd = ((m+5.0f - i) / m);
			Transform& t = scene.GetComponent<Transform>(cubeList[i]);
			float val = (sin((i+time*2.0f)))*5;
		
			t.position.y = (val*fixEnd+sin(i/10.0f+time*2)*5)*fixSt;

			t.scale = glm::vec3(1, 1, 1) *(fixEnd);
			
			
			t.position.z = sin((0.5+i)/2+time*2.0f)*fixSt*10 ;
			
		}
	}



	return 0;
}





