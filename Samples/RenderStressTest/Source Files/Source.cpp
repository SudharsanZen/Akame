#define _CRTDBG_MAP_ALLOC
#define AK_PRIVATE_GETTER_SETTER
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

class rotate :public Behaviour
{

public:
	rotate()
	{
		
	}
	void OnStart() override
	{
	}
	void Update(float deltaTime) override
	{

		
		Transform& t = GetComponent<Transform>();

		Lights& l = GetComponent<Lights>();
	
		Quaternion rot = t.GetGlobalRotation();
		glm::vec3 pose = t.GetGlobalPosition();
		float speed = 10;
		t.SetGlobalRotation(rot*Quaternion(speed*deltaTime,speed*deltaTime,speed*deltaTime));

		Mesh& m = GetComponent<Mesh>();
		auto _min = m.GetMin();
		auto _max = m.GetMax();
		auto off_set_origin = (_min+_max)/2.0f;

		glm::vec3 origin = t.transformMatrix()*glm::vec4(off_set_origin,1);
		auto half_extent_s = glm::abs(_max-off_set_origin)*t.GetGlobalScale();

		Debug::DrawRay(origin, t.forward()*half_extent_s.x, 1, glm::vec3(0, 0, 1));
		Debug::DrawRay(origin, t.up() * half_extent_s.y, 1, glm::vec3(0, 1, 0));
		Debug::DrawRay(origin, t.right() * half_extent_s.z, 1, glm::vec3(1, 0, 0));
		float x = fabs(fabs(t.right().x*half_extent_s.x)+fabs(t.up().x*half_extent_s.y)+fabs(t.forward().x*half_extent_s.z));
		float y = fabs(fabs(t.right().y*half_extent_s.x)+fabs(t.up().y*half_extent_s.y)+fabs(t.forward().y*half_extent_s.z));
		float z = fabs(fabs(t.right().z*half_extent_s.x)+fabs(t.up().z*half_extent_s.y)+fabs(t.forward().z*half_extent_s.z));
		glm::vec3 half_extent = glm::vec3(x,y,z);
		Debug::DrawBB(origin-half_extent,origin+half_extent,glm::vec3(0,1,0));
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

	Entity dir = scene.CreateEntity();
	Lights& d = scene.AddComponent<Lights>(dir);
	d.setType(LIGHT::DIRECTIONAL);
	d.setColor(1, 1, 1);
	d.setDirection(-90, 0, 0);
	d.setIntensity(3);
	d.setPointLightConst(1, 2, 10);
	d.ambientLigting(0.1f, 0.1f, 0.1f);

	scene.AddComponent<Transform>(dir);
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

	

	
	Material mat("GRIDS");
	Entity pl = scene.CreateEntity();


	scene.AddComponent<Transform>(pl);
	Mesh &plm=scene.AddComponent<Mesh>(pl);
	scene.AddComponent<Material>(pl)= mat;
	plm.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	/*Material matS("SPHERE");
	Entity sky = scene.CreateEntity();



	scene.AddComponent<Transform>(sky);
	Mesh &skym=scene.AddComponent<Mesh>(sky);
	scene.AddComponent<Material>(sky)=matS;
	skym.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	*/
	
	const int rt = (int)sqrt(num);
	
	std::vector<Entity> lightsVec;
	auto b = generateCubeVertices();
	for (int i = 0; i < b.size(); i++)
	{
		b[i].pos.x *= 2.0f;
		b[i].pos.x -= 4.0f;
		
	}
	for (int i = 0; i < 1; i++)
	{
		float off = (float)(rt - 1);

		/*
		Entity point = scene.CreateEntity();
		Lights& p = scene.AddComponent<Lights>(point);

		p.setColor(1, 1, 1);
		p.setIntensity(4);
		p.setPointLightConst(1, 2, 5);
		p.ambientLigting(0, 0, 0);

		scene.AddComponent<Transform>(point).SetGlobalPosition(glm::vec3(2 * (i / rt) - 1 - off, 0.5, (2) * (i % rt) - off));
		scene.AddComponent<BehaviourComponent>(point).setBehaviour<strafe>(float(i));
		lightsVec.push_back(point);
		*/
		Entity box = scene.CreateEntity();
		Mesh& bm = scene.AddComponent<Mesh>(box);
		int curr = (i / rt) % 2;
		//if (curr == 1)
			bm.CreateMesh(b);
		//if (curr == 0)
			//bm.CreateMesh(generateSphereVertices(16, 32, 0.5));
		Transform& t = scene.AddComponent<Transform>(box);
		t.SetGlobalPosition(glm::vec3(0*2.0f * (i / rt) - off*0.0f, 0.5f, 0*(2.0f) * (i % rt) - off*0.0f));
		t.SetGlobalScale(glm::vec3(5));

		if (curr == 0)
		{
			int a = rand() % 2;
			
			scene.AddComponent<Material>(box) = rust1;
		}
		else
			scene.AddComponent<Material>(box) = boxMat;
		scene.AddComponent<BehaviourComponent>(box).setBehaviour<rotate>();
		scene.GetComponent<Transform>(box).SetGlobalRotation(Quaternion(0, 0, 0));
	}
	//Editor is experimental, do not use this
	Editor edt(scene);
	scene.OnStart();
	scene.vsyncOn(false);
	scene.backGroundColor(0, 0, 0, 1);
	float step = 0.3f;
	float acc = 0;
	bool stopRot = false;

	while (!window.closeWindow())
	{
		if (stopRot)
			acc += scene.getDeltaTime();

		//flyCam(scene.cam, scene.getDeltaTime());
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		scene.clearBuffer();
		//scene.Render();
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
		app(10000);
	}
	//AssetManager::reset();
	_CrtDumpMemoryLeaks();
	return 0;
}





