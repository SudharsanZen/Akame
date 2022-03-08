#define EDITOR_MODE
#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include"Core/Engine.h"
#include"Core/Scene.h"
#include"Core/Window.h"
#include<vector>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include"Core/Debug/Debug.h"
#include<Rendering/DeferredRendererFragmentBuffer.h>
#include"Core/Serialization/SceneSerialization.h"
class rotateBehv :public Behaviour
{
	float angle = 180.0f;
public:
	void OnStart() override
	{
	}
	void Update(float deltaTime) override
	{

		angle += deltaTime * 5.0f;
		angle = angle - floor(angle / 360.0f) * 360.0f;
		Transform& t = GetComponent<Transform>();
		t.SetGlobalRotation(Quaternion::rotationAroundAxisVector(angle, glm::vec3(0, 1, 0)));
	}
};

class strafe :public Behaviour
{
	float acc = 0;
	float offSet;
public:
	strafe(float off)
	{
		offSet = off;
	}
	void OnStart() override
	{
	}
	void Update(float deltaTime) override
	{
		acc += deltaTime;
		Transform& t = GetComponent<Transform>();
		Lights& l = GetComponent<Lights>();
		//Debug::DrawCircle(t.GetGlobalPosition(),worldUp,l.getPointLightRadius(),glm::vec3(1,0,0),20);
		glm::vec3 pose = t.GetGlobalPosition();
		t.SetGlobalPosition(glm::vec3(pose.x, pose.y, sin(offSet + acc) * 3.0f));
	}
};

int main()
{
	AssetManager::setAssetRoot("../../../Assets/");
	std::string rootDir(AssetManager::getAssetRoot());
	Window window(1080, 720, "testWindow");

	if (!window.initialize())
	{
		std::cout << "Can't init WINDOW";
		return -1;
	}
	Scene scene(window);
	scene.cam.setFar(1000);
	/*
	Material boxMat("DEFERRED");
	boxMat.setTexture2D("material.diffuse", rootDir + "Media/pbr/crate/basecolor.jpg");
	boxMat.setTexture2D("material.roughness", rootDir + "Media/pbr/crate/roughness.jpg");
	boxMat.setTexture2D("material.normal", rootDir + "Media/pbr/crate/normal.jpg");

	boxMat.setValue("noMetallic", 1.0f);
	boxMat.setValue("noRougness", 0.0f);
	boxMat.setValue("noAO", 1);
	boxMat.setValue("metallic", 0.04f);
	boxMat.setValue("ambientocclusion", 0.04f);
	boxMat.setValue("normalStrength", 1.0f);
	
	Entity sphere = scene.CreateEntity();
	Mesh& bm = scene.AddComponent<Mesh>(sphere);
	bm.CreateMesh(generateSphereVertices(16, 32, 0.5));
	Transform& t = scene.AddComponent<Transform>(sphere);
	t.SetGlobalPosition(glm::vec3(0, 10, 0));
	scene.AddComponent<Material>(sphere) = boxMat;
	scene.GetComponent<Transform>(sphere).SetGlobalRotation(Quaternion(0, 0, 0));
	
	Material planeMat("DEFERRED");
	planeMat.setTexture2D("material.diffuse", rootDir + "Media/pbr/rust/diffuse.png");
	planeMat.setTexture2D("material.roughness", rootDir + "Media/pbr/rust/roughness.png");
	planeMat.setTexture2D("material.normal", rootDir + "Media/pbr/rust/normal.png");
	planeMat.setTexture2D("material.metallic", rootDir + "Media/pbr/rust/metallic.png");
	planeMat.setValue("noAO", 1);
	planeMat.setValue("noRoughness", 0);
	planeMat.setValue("ambientocclusion", 1);
	planeMat.setValue("noMetallic", 0);
	planeMat.setValue("normalStrength", 1);


	Entity plane = scene.CreateEntity();


	Mesh& mP = scene.AddComponent<Mesh>(plane);
	scene.AddComponent<Material>(plane) = planeMat;
	mP.CreateMesh(generatePlaneVertices());
	Transform& tp = scene.AddComponent<Transform>(plane);
	tp.SetGlobalScale(30.0f * glm::vec3(1));
	tp.SetGlobalPosition(glm::vec3(0, 1, 0));





	Entity dir = scene.CreateEntity();
	Lights& d = scene.AddComponent<Lights>(dir);
	d.setType(LIGHT::DIRECTIONAL);
	d.setColor(1, 1, 1);
	d.setDirection(-90, 0, 0);
	d.setIntensity(1);
	d.setPointLightConst(1, 2, 10);
	d.ambientLigting(0.1f, 0.1f, 0.1f);

	scene.AddComponent<Transform>(dir);

	Model mLoader(scene);
	/*
	Entity model = mLoader.LoadModelToScene(rootDir + "Media/Erika/Catwalk Walk Turn 180 Tight.fbx");
	//scene.AddComponent<BehaviourComponent>(bag).setBehaviour<rotateBehv>();
	Transform& T = scene.GetComponent<Transform>(model);
	T.SetGlobalScale(glm::vec3(0.02f));
	T.SetGlobalPosition(glm::vec3(0, 1, 0));
	



	Material mat("GRIDS");
	Entity pl = scene.CreateEntity();


	scene.AddComponent<Transform>(pl);
	Mesh& plm = scene.AddComponent<Mesh>(pl);
	scene.AddComponent<Material>(pl) = mat;
	plm.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);

	Material matS("SPHERE");
	Entity sky = scene.CreateEntity();



	scene.AddComponent<Transform>(sky);
	Mesh& skym = scene.AddComponent<Mesh>(sky);
	scene.AddComponent<Material>(sky) = matS;
	skym.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);

	*/

	//Editor is experimental, do not use this
	Editor e(scene);

	float acc = 0;
	scene.OnStart();
	scene.vsyncOn(true);
	unsigned int count = 0;
	scene.backGroundColor(1, 1, 1, 1);
	float step = 0.3f;

	/*
	Entity box = scene.CreateEntity();
	Transform &boxT=scene.AddComponent<Transform>(box);
	boxT.SetGlobalScale(glm::vec3(4));
	Mesh &boxMesh=scene.AddComponent<Mesh>(box);
	boxMesh.CreateMesh(generateCubeVertices());
	scene.AddComponent <Material> (box)=boxMat;
	*/
	scene.backGroundColor(0.1f,0.1f,0.1f,0.1f);
	SceneDeserializer sd(scene);
	sd.LoadFrom(AssetManager::assetRootPath+"Media\\ExportTest\\Scenes\\test6.ascene");
	while (!window.closeWindow())
	{

		//t.SetGlobalPosition(glm::vec3(5, fabs(sin(acc)) * 30, 0));
		acc += scene.getDeltaTime() / 3;
		flyCam(scene.cam, scene.getDeltaTime());
		scene.clearBuffer();
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		//scene.Render();
		e.DrawUI();

		scene.swapBuffers();


	}



	return 0;
}





