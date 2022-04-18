#include<iostream>

#define AK_PRIVATE_GETTER_SETTER
#include<fstream>
#include<string>
#include<algorithm>
#include<vector>
#include<stdlib.h>
#include<Core/Engine.h>
#include<Rendering/DeferredRendererFragmentBuffer.h>
#include<Core/Scene.h>
#include<Core/Debug/Debug.h>
#include<Core/Reflection/ReflectionUIHandler.h>
#include<Core/Reflection/ReflectionMeta.h>
#define sizeX 10.0f
#define sizeY 10.0f
#include"Rendering/System/RenderingSystem.h"

#include"Math/Octree.h"
#include"Rendering/FrustumCuller.h"

class strafeRandom:public Behaviour
{

private:
	int* level;
	AK_SHOW_IN_INSPECTOR
	(
		AK_ID(*level)
		AK_ID(baseSpeed)
		AK_ID(speedMultiplier)
	)
	glm::vec3 moveDir;
	static float baseSpeed;
	static float speedMultiplier;
	float speed;
	float time;
public:
	void OnStart()override
	{
		time = 0.0f;
	}
	strafeRandom(float pose,int *treeDepth)
	{
		level = treeDepth;
		speed =  rand() % 20;
		moveDir = worldForward*Quaternion(pose,pose*30,pose+60);
		moveDir = glm::normalize(moveDir);
	}
	void Update(float delta)override
	{
		time += delta;
		//Transform& trans = GetComponent<Transform>();
		//trans.SetGlobalPosition(trans.GetGlobalPosition()+moveDir*(speed+baseSpeed)*speedMultiplier*sin(time*1)*delta);
	}
};
float strafeRandom::baseSpeed = 10;
float strafeRandom::speedMultiplier = 1;






class DummyCamController:public Behaviour
{
	Camera& m_cam;
	glm::vec3 pose;
	glm::vec3 angle;
	float fov;
	float far;
public:
	AK_SHOW_IN_INSPECTOR
		(
			AK_ID_COMPX(pose)
			AK_ID_COMPX(angle)
			AK_ID_COMPX(fov)
			AK_ID_COMPX(far)
		)
	DummyCamController(Camera& m_cam) :m_cam(m_cam)
	{
		pose = glm::vec3(0);
		angle = glm::vec3(0);
		fov = 11;
		far = 1000;
	}

	void Update(float detlatime) override
	{
		m_cam.setCameraPosition(pose);
		m_cam.setCameraRotation(angle);
		m_cam.setFieldOfView(fov);
		m_cam.setFar(far);
	}
	void OnStart()override
	{
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
	
	Material cmat("DEFERRED");
	cmat.setTexture2D("material.diffuse", rootDir + "Media/pbr/rust1/diffuse.png");
	cmat.setTexture2D("material.roughness", rootDir + "Media/pbr/rust1/roughness.png");
	cmat.setTexture2D("material.normal", rootDir + "Media/pbr/rust1/normal.png");
	cmat.setTexture2D("material.metallic", rootDir + "Media/pbr/rust1/metallic.png");
	cmat.setValue("noAO", 1);
	cmat.setValue("ambientocclusion", 1);
	cmat.setValue("noMetallic", 0);
	cmat.setValue("normalStrength", 1);

	Scene m_scene(window);
	
	Entity dir = m_scene.CreateEntity();
	Lights &d = m_scene.AddComponent<Lights>(dir);
	d.setType(LIGHT::DIRECTIONAL);
	d.setColor(1, 1, 1);
	d.setDirection(-90,0,0);
	d.setIntensity(0.6f);
	d.setPointLightConst(1,2,10);
	d.ambientLigting(0.1f,0.1f,0.1f);
	
	m_scene.AddComponent<Transform>(dir).SetGlobalPosition(glm::vec3(0,2,0));
	

	std::vector<Entity> cubeList;
	float m = 100;
	float dist = 100.0f;

	float totalObj = m*m*m;
	float half = dist / 2.0f;
	float distperobj = dist/m;
	int* level=new int;
	*level=5;
	OctTreeNode::SetMaxLevel(5);
	OctTreeNode::SetMinLevel(-4);
	for (int i = 0; i < m*m; i++)
	{

		Entity cube = m_scene.CreateEntity();
		cubeList.push_back(cube);
		Transform& t = m_scene.AddComponent<Transform>(cube);
		t.SetGlobalScale(glm::vec3(2.0f));
		t.SetGlobalRotation(Quaternion(0,0,0));
		float nX = (float)((int)(i/m)%(int)(m))*distperobj-half;
		float nY = static_cast<int>(i / (m * m)) * distperobj - half;
		float nZ = static_cast<int>(i%static_cast<int>(m))*distperobj-half;
		t.SetGlobalPosition(glm::vec3(((int)(i/(int)m))*4.0f-((float)m/2.0f),4,((i)%(int)m) * 4.0f-((float)m)));
		//t.SetGlobalPosition(glm::vec3(nX,nY,nZ));
		Mesh &cM=m_scene.AddComponent<Mesh>(cube);
		m_scene.AddComponent<Material>(cube)=cmat;
		cM.CreateMesh(generateCubeVertices());
		//auto& beh=m_scene.AddComponent<BehaviourComponent>(cube);
		//beh.setBehaviour<strafeRandom>(i%360,level);
		//if(nX>0&& nZ>0 && nY>0)

	}
	
	/*
	Material matS("SPHERE");
	Entity sky = m_scene.CreateEntity();



	m_scene.AddComponent<Transform>(sky);
	Mesh& skym = m_scene.AddComponent<Mesh>(sky);
	m_scene.AddComponent<Material>(sky) = matS;
	skym.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	
	Material mat("GRIDS");
	Entity plane= m_scene.CreateEntity();

	m_scene.AddComponent<Transform>(plane);
	Mesh &pm=m_scene.AddComponent<Mesh>(plane);
	m_scene.AddComponent<Material>(plane)=mat;
	pm.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	
	*/

	
	

	//Editor is experimental, do not use this
	Editor edt(m_scene);
	m_scene.OnStart();
	m_scene.vsyncOn(false);
	float deltaTime = 0;


	
	Camera m_cam(60,1,0.1f,1000.0f);
	m_cam.setCameraPosition(0,0,0);
	FrustumCuller frustum(m_cam);
	auto& beh = m_scene.AddComponent<BehaviourComponent>(dir);
	beh.setBehaviour<DummyCamController>(m_cam);
	while (!window.closeWindow())
	{




		deltaTime = m_scene.getDeltaTime();
		//flyCam(m_scene.cam,m_scene.getDeltaTime());
		//m_scene.m_cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		m_scene.clearBuffer();
		//m_scene.Render();
	
		

		edt.DrawUI();

		m_scene.swapBuffers();

	}

	

	return 0;
}





