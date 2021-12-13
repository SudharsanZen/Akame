#define _CRTDBG_MAP_ALLOC
#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include"Core/Engine.h"
#include"Core/Scene.h"
#include<vector>
#include<sstream>
#include <stdlib.h>
#include <crtdbg.h>
#include"Rendering/System/PSSMFBO.h"
#include"Core/Debug/Debug.h"

#include"Rendering/DeferredRendererFragmentBuffer.h"
#include"Core/Editor/EditorUI.h"
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
	
		glm::vec3 pose = t.GetGlobalPosition();
		t.SetGlobalPosition(glm::vec3(pose.x, 1.0f*fabs(sin(offSet + acc)), pose.z));
	}
};
void app(int num=200)
{

	AssetManager::setAssetRoot("../../../../Assets/");
	std::string rootDir(AssetManager::getAssetRoot());
	Window window(800, 800, "testWindow");
	std::cout << sizeof(glm::mat4);
	if (!window.initialize())
	{
		std::cout << "Can't init WINDOW";

	}
	Scene scene(window);


	Entity plane = scene.CreateEntity();
	Entity plane2 = scene.CreateEntity();

	Material boxMat("DEFERRED");
	boxMat.setTexture2D("material.diffuse", rootDir + "Media/pbr/crate/basecolor.jpg");
	boxMat.setTexture2D("material.roughness", rootDir + "Media/pbr/crate/roughness.jpg");
	boxMat.setTexture2D("material.normal", rootDir + "Media/pbr/crate/normal.jpg");
	boxMat.setTexture2D("material.ambientocclusion",rootDir+"Media/pbr/crate/ambientocclusion.jpg");
	boxMat.setValue("noAO", 0);
	boxMat.setValue("noMetallic", 1);
	boxMat.setValue("metallic", 0.04f);
	boxMat.setValue("normalStrength", 1);

	scene.vsyncOn(true);
	Material planeMat("DEFERRED");
	planeMat.setTexture2D("material.diffuse", rootDir + "Media/pbr/rust/diffuse.png");
	planeMat.setTexture2D("material.roughness", rootDir + "Media/pbr/rust/roughness.png");
	planeMat.setTexture2D("material.normal", rootDir + "Media/pbr/rust/normal.png");
	planeMat.setTexture2D("material.metallic", rootDir + "Media/pbr/rust/metallic.png");
	planeMat.setValue("noAO", 1);
	planeMat.setValue("ambientocclusion",1);
	planeMat.setValue("noMetallic", 0);
	planeMat.setValue("normalStrength", 1);

	Material rust1("DEFERRED");
	rust1.setTexture2D("material.diffuse", rootDir + "Media/pbr/rust1/diffuse.png");
	rust1.setTexture2D("material.roughness", rootDir + "Media/pbr/rust1/roughness.png");
	rust1.setTexture2D("material.normal", rootDir + "Media/pbr/rust1/normal.png");
	rust1.setTexture2D("material.metallic", rootDir + "Media/pbr/rust1/metallic.png");
	rust1.setValue("noAO", 1);
	rust1.setValue("ambientocclusion", 1);
	rust1.setValue("noMetallic", 0);
	rust1.setValue("normalStrength", 1);



	Mesh& mP = scene.AddComponent<Mesh>(plane);
	mP.CreateMesh(generatePlaneVertices());
	Transform& t = scene.AddComponent<Transform>(plane);
	t.SetGlobalScale(30.0f* glm::vec3(1));
	t.SetGlobalPosition(glm::vec3(0,0.01,0));
	scene.AddComponent<Material>(plane) = planeMat;

	physics::RigidBody3D& rbdy2 = scene.AddComponent<physics::RigidBody3D>(plane);
	physics::ColliderShape planeShape;
	planeShape.setColliderShape(physics::Shapes::PLANE, 30, 30);
	rbdy2.setRigidBodyType(physics::RigidBodyType::STATIC, planeShape);



	Mesh& plane2M = scene.AddComponent<Mesh>(plane2);
	plane2M.CreateMesh(generatePlaneVertices());
	scene.AddComponent<Transform>(plane2);
	scene.AddComponent<Material>(plane2) = boxMat;

	Entity dir = scene.CreateEntity();
	Lights& d = scene.AddComponent<Lights>(dir);
	d.setType(LIGHT::DIRECTIONAL);
	d.setColor(1, 1, 1);
	d.setDirection(1, -1, 0);
	d.setIntensity(3);
	d.setPointLightConst(1, 2, 10);
	d.ambientLigting(0.1f, 0.1f, 0.1f);

	scene.AddComponent<Transform>(dir);

	
	Material mat("GRIDS");
	Entity pl = scene.CreateEntity();


	scene.AddComponent<Transform>(pl);
	Mesh &plm=scene.AddComponent<Mesh>(pl);
	scene.AddComponent<Material>(pl)= mat;
	plm.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	Material matS("SPHERE");
	Entity sky = scene.CreateEntity();



	scene.AddComponent<Transform>(sky);
	Mesh &skym=scene.AddComponent<Mesh>(sky);
	scene.AddComponent<Material>(sky)=matS;
	skym.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	
	
	const int rt = (int)sqrt(num);
	/*
	std::vector<Entity> lightsVec;
	std::vector<vert> cv = generateCubeVertices();
	for (int i = 0; i < num; i++)
	{
		float off = (float)(rt - 1);


		Entity point = scene.CreateEntity();
		Lights& p = scene.AddComponent<Lights>(point);

		p.setColor(1, 1, 1);
		p.setIntensity(4);
		p.setPointLightConst(1, 2, 5);
		p.ambientLigting(0, 0, 0);

		scene.AddComponent<Transform>(point).SetGlobalPosition(glm::vec3(2 * (i / rt) - 1 - off, 0.5, (2) * (i % rt) - off));
		scene.AddComponent<BehaviourComponent>(point).setBehaviour<strafe>(float(i));
		lightsVec.push_back(point);

		Entity box = scene.CreateEntity();
		Mesh& bm = scene.AddComponent<Mesh>(box);
		int curr = (i / rt) % 2;
		if (curr == 1)
			bm.CreateMesh(cv);
		if (curr == 0)
			bm.CreateMesh(generateSphereVertices(16, 32, 0.5));
		Transform& t = scene.AddComponent<Transform>(box);
		t.SetGlobalPosition(glm::vec3(2.0f * (i / rt) - off, 0.5f, (2.0f) * (i % rt) - off));

		if (curr == 0)
		{
			int a = rand() % 2;
			
			scene.AddComponent<Material>(box) = (a==0)?rust1:planeMat;
		}
		else
			scene.AddComponent<Material>(box) = boxMat;

		scene.GetComponent<Transform>(box).SetGlobalRotation(Quaternion(0, 0, 0));
	}*/
	Editor edt(window,scene);
	scene.OnStart();
	scene.vsyncOn(true);
	scene.backGroundColor(0, 0, 0, 1);
	float step = 0.3f;
	float acc = 0;
	bool stopRot = false;

	while (!window.closeWindow())
	{
		if (stopRot)
			acc += scene.getDeltaTime();

		flyCam(scene.cam, scene.getDeltaTime());
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		scene.clearBuffer();
		scene.Render();
		edt.DrawUI();

		/*
		//debug draw, point light
		for (int i = 0; i < lightsVec.size(); i++)
		{
			Lights &l=scene.GetComponent<Lights>(lightsVec[i]);
			Transform &T=scene.GetComponent<Transform>(lightsVec[i]);
			Debug::DrawCircle(T.GetGlobalPosition(),worldUp,l.getPointLightRadius());
		}
		*/

		scene.swapBuffers();
	}


}
int main(int argc,char **argv)
{
	for (int i = 0; i < argc; i++)
	{
		std::cout << argv[i];
	}
	if (argc > 1)
	{
		std::stringstream ss(argv[1]);
		int i;
		ss>>i;
		app(i);
	}
	else
	{
		app();
	}
	//AssetManager::reset();
	_CrtDumpMemoryLeaks();
	return 0;
}





