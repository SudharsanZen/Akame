#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<vector>
#include<stdlib.h>
#include<Core/Engine.h>
#include<Rendering/DeferredRendererFragmentBuffer.h>
#include<Core/Scene.h>
#include<Core/Debug/Debug.h>
#include<Core/Reflection/Reflection.h>
#define sizeX 10.0f
#define sizeY 10.0f
class rotateBehv :public Behaviour
{
	float angle = 180.0f;
	glm::vec3 pose;
	bool spinFast=false;
public:
	AK_SERIALIZABLES
	(
		AK_ID(angle)
	)

	AK_SHOW_IN_INSPECTOR
	(
		AK_ID(angle)
		AK_ID(pose)
		AK_ID(spinFast)
	)
	void OnStart() override
	{
		pose = glm::vec3(1,0,0);
	}
	void Update(float deltaTime) override
	{

		angle += deltaTime * ((spinFast)?10.0f:5.0f);
		angle = angle - floor(angle / 360.0f) * 360.0f;
		Transform& t = GetComponent<Transform>();
		t.SetGlobalRotation(Quaternion::rotationAroundAxisVector(angle, glm::vec3(0, 1, 0)));
		t.SetGlobalPosition(pose);
	}
};

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

	Material cmat("DEFAULT");
	cmat.setTexture2D("material.diffuse", AssetManager::assetRootPath + "Media/pbr/basecolor.jpg");
	cmat.setTexture2D("material.normal", AssetManager::assetRootPath + "Media/pbr/normal.jpg");
	cmat.setTexture2D("material.specular", AssetManager::assetRootPath + "Media/pbr/roughness.jpg");

	Scene scene(window);
	Entity dir = scene.CreateEntity();
	Lights &d = scene.AddComponent<Lights>(dir);
	d.setType(LIGHT::DIRECTIONAL);
	d.setColor(1, 1, 1);
	d.setDirection(-90,0,0);
	d.setIntensity(0.6f);
	d.setPointLightConst(1,2,10);
	d.ambientLigting(0.1f,0.1f,0.1f);
	
	scene.AddComponent<Transform>(dir).SetGlobalPosition(glm::vec3(0,2,0));
	std::vector<Entity> cubeList;
	float m = 100;
	for (int i = 0; i < m; i++)
	{
		
		
		Entity cube = scene.CreateEntity();
		cubeList.push_back(cube);
		Transform& t = scene.AddComponent<Transform>(cube);
		t.SetGlobalRotation(Quaternion(0,0,0));
		t.SetGlobalPosition(glm::vec3(float(i), 0, 0));
		Mesh &cM=scene.AddComponent<Mesh>(cube);
		scene.AddComponent<Material>(cube)=cmat;
		cM.CreateMesh(generateSphereVertices(32,16,0.5));

		if (i > 0)
		{
			t.setParent(cubeList[i - 1]);
			
		}
		else
		{

			t.SetLocalScale(glm::vec3(3));
			//t.SetGlobalRotation(Quaternion::rotationAroundAxisVector(30,glm::vec3(0,0,1)));
		}
	}
	

	for (int i = 1; i < m; i++)
	{
		Transform& t = scene.GetComponent<Transform>(cubeList[i]);
		t.SetLocalRotation(Quaternion(0, ((float)i / float(m)) *50, 0));
	}
	/*
	Material mat("GRIDS");
	Entity plane= scene.CreateEntity();

	scene.AddComponent<Transform>(plane);
	Mesh &pm=scene.AddComponent<Mesh>(plane);
	scene.AddComponent<Material>(plane)=mat;
	pm.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	
	
	Material matS("SPHERE");
	Entity sky= scene.CreateEntity();
	scene.AddComponent<Material>(sky) = matS;
	scene.AddComponent<Transform>(sky);
	Mesh& skMesh = scene.AddComponent<Mesh>(sky);
	skMesh.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	*/
	
	Transform& t = scene.GetComponent<Transform>(cubeList[0]);
	Transform& t1 = scene.GetComponent<Transform>(cubeList[4]);
	
	t.SetGlobalRotation(Quaternion::rotationAroundAxisVector(30, glm::vec3(0, 0, 1)));

	BehaviourComponent &b=scene.AddComponent<BehaviourComponent>(cubeList[0]);
	b.setBehaviour<rotateBehv>();
	//Editor is experimental, do not use this
	Editor edt(scene);
	scene.OnStart();
	scene.vsyncOn(false);
	float deltaTime = 0;
	while (!window.closeWindow())
	{
		deltaTime = scene.getDeltaTime();
		flyCam(scene.cam,scene.getDeltaTime());
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		scene.clearBuffer();
		//scene.Render();
		edt.DrawUI();
		glm::vec3 pose = t.GetLocalPosition();
		//t.SetLocalScale(-1.0f* glm::vec3(1));
		//t1.SetGlobalScale(sin(time)*glm::vec3(1));
		//t.SetGlobalRotation(Quaternion::rotationAroundAxisVector(time,glm::vec3(0,0,1)));
		
		//t1.SetGlobalRotation(Quaternion::rotationAroundAxisVector(deltaTime, glm::vec3(0, 1, 0))*t.GetGlobalRotation());
		scene.swapBuffers();
		for (int i = 0; i < m; i++)
		{
			
			
			
				Transform& tr = scene.GetComponent<Transform>(cubeList[i]);
				Debug::DrawRay(tr.GetGlobalPosition(), tr.forward(), 1, glm::vec3(0, 0, 1));
				Debug::DrawRay(tr.GetGlobalPosition(), tr.up(), 1, glm::vec3(0, 1, 0));
				Debug::DrawRay(tr.GetGlobalPosition(), tr.right(), 1, glm::vec3(1, 0, 0));
			
		}
	}



	return 0;
}





