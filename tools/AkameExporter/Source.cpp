#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<vector>
#include<stdlib.h>
#include<Core/Engine.h>
#include<Rendering/DeferredRendererFragmentBuffer.h>
#include<Core/Scene.h>
#include<sstream>
#include"Core/Editor/EditorUI.h"
#define sizeX 10.0f
#define sizeY 10.0f

int main()
{
	AssetManager::setAssetRoot("../../../../Assets/");
	std::string rootDir(AssetManager::getAssetRoot());
	Window window(800, 800, "DUMBSTUFF");

	if (!window.initialize())
	{
		std::cout << "Can't init WINDOW";
		return -1;
	}



	Scene scene(window);
	EntityID dir = scene.CreateEntity();
	Lights& d = scene.AddComponent<Lights>(dir);
	d.setType(LIGHT::DIRECTIONAL);
	d.setColor(1, 1, 1);
	d.setDirection(1, -1, 0);
	d.setIntensity(0.6f);
	d.setPointLightConst(1, 2, 10);
	d.ambientLigting(0.1f, 0.1f, 0.1f);

	scene.AddComponent<Transform>(dir).SetGlobalPosition(glm::vec3(0, 2, 0));
	scene.SetEntityName(dir, "DirectionalLight");
	std::vector<EntityID> cubeList;
	float m = 5;
	int i;
	{
		std::shared_ptr<Material> cmat = std::make_shared<Material>("DEFERRED");
		cmat->setTexture2D("material.diffuseMap", AssetManager::assetRootPath + "Media/pbr/basecolor.jpg");
		cmat->setTexture2D("material.normalMap", AssetManager::assetRootPath + "Media/pbr/normal.jpg");
		cmat->setTexture2D("material.specularMap", AssetManager::assetRootPath + "Media/pbr/roughness.jpg");
		for (i = 0; i < m; i++)
		{


			EntityID cube = scene.CreateEntity();
			cubeList.push_back(cube);
			Transform& t = scene.AddComponent<Transform>(cube);
			t.SetGlobalRotation(Quaternion(0, 0, 0));
			t.SetGlobalPosition(glm::vec3(float(i), 0, 0));
			scene.AddComponent<Material>(cube) = *cmat;
			Mesh& cM = scene.AddComponent<Mesh>(cube);

			cM.CreateMesh(generateSphereVertices(32, 16, 0.5));

			if (i > 0)
			{
				t.setParent(cubeList[i - 1]);
			}
			else
			{

				t.SetLocalScale(glm::vec3(3));
				t.SetGlobalRotation(Quaternion::rotationAroundAxisVector(30, glm::vec3(0, 0, 1)));
			}
			std::stringstream str;
			str << "Entity: " << i;

			scene.SetEntityName(cube, str.str());
		}
		cmat.reset();
	}



	Material mat("GRIDS");
	EntityID plane = scene.CreateEntity();
	Transform planeInf;

	scene.AddComponent<Transform>(plane) = planeInf;
	Mesh& pm = scene.AddComponent<Mesh>(plane);
	scene.AddComponent<Material>(plane) = mat;
	pm.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	scene.SetEntityName(plane, "grids");

	Material matS("SPHERE");
	EntityID sky = scene.CreateEntity();
	scene.AddComponent<Material>(sky) = matS;
	scene.AddComponent<Transform>(sky);
	Mesh& skMesh = scene.AddComponent<Mesh>(sky);
	skMesh.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	scene.SetEntityName(sky, "skyquad");

	Transform& t = scene.GetComponent<Transform>(cubeList[0]);
	Transform& t1 = scene.GetComponent<Transform>(cubeList[4]);

	//t.SetGlobalRotation(Quaternion::rotationAroundAxisVector(30, glm::vec3(0, 0, 1)));

	Editor editor(window, scene);
	scene.OnStart();
	scene.vsyncOn(false);
	float deltaTime = 0;
	i = 3;
	while (!window.closeWindow())
	{

		deltaTime = scene.getDeltaTime();
		flyCam(scene.cam, scene.getDeltaTime());
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		scene.clearBuffer();

		scene.Render();

		editor.DrawUI();
		glm::vec3 pose = t.GetLocalPosition();

		t.SetGlobalRotation(Quaternion::rotationAroundAxisVector(deltaTime * 10, glm::vec3(0, 1, 0)) * t.GetGlobalRotation());

		scene.swapBuffers();


	}


	std::cout << "total count:" << i;
	return 0;
}




