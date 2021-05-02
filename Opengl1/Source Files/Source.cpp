#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include"Engine.h"
#include"Editor/Scene.h"
#include<vector>

const float reduceDistFact =10;
float kmToSceneUnit(float km)
{
	//1 km is 0.0001 scene unit
	return km / 10000.0f;
}
class EarthBehv :public Behaviour
{
	float y = 0;
	float sunEarthAngle = 0;
	float sunEarthDist =200;
	EntityID sun;
	std::shared_ptr<Scene> scene;
public:
	EarthBehv(EntityID eid,EntityID sun,std::shared_ptr<Scene> scene):Behaviour(eid)
	{
		this->scene = scene;
		this->sun = sun;
	}

	void OnStart()override
	{

	}

	void Update(float deltaTime)override
	{
		y += 1*deltaTime*10;
		sunEarthAngle += deltaTime*5;
		if (y > 360)
			y = 0;
		if (sunEarthAngle > 360)
			sunEarthAngle = 0;

		scene->GetComponent<Transform>(entityID).rotation.setEulerAngle(0,y,0);




		Quaternion rot = Quaternion::rotationAroundAxisVector(sunEarthAngle, worldUp);
		glm::vec3 sunPosition = scene->GetComponent<Transform>(sun).position;

		scene->GetComponent<Transform>(entityID).position = sunPosition + rot * glm::vec3(0, 0, 1)*sunEarthDist;//38.4400
	}
};

class MoonBehv :public Behaviour
{
	std::shared_ptr<Scene> scene;
	EntityID earth;
	float y = 0;
	float eartMoonAng=0;
	float distanceFromEarth =30;
public:
	MoonBehv(EntityID eid,EntityID earth, std::shared_ptr<Scene> scene) :Behaviour(eid)
	{
		this->earth = earth;
		this->scene = scene;
	}
	void OnStart()override
	{

	}

	void Update(float deltaTime)override
	{
		eartMoonAng += deltaTime * 100;
		y += 1 * deltaTime * 10;
		if (y > 360)
			y = 0;
		if (eartMoonAng > 360)
			eartMoonAng = 0;

		scene->GetComponent<Transform>(entityID).rotation.setEulerAngle(0, y, 0);

		Quaternion rot=Quaternion::rotationAroundAxisVector(eartMoonAng,worldUp);
		glm::vec3 earthPosition=scene->GetComponent<Transform>(earth).position;
		
		scene->GetComponent<Transform>(entityID).position = earthPosition+rot*glm::vec3(0,0,1)*distanceFromEarth;//38.4400
		
	}
};


int main()
{
	//0.0001 unit in the scene is 1 km in real world
	//12,742km		1.2742
	Window window(800,800,"Kunal");
	
	if (!window.initialize())
		std::cout << "Something went wrong, can't initialize window";
	
	std::shared_ptr<Scene> scene=std::make_shared<Scene>(window);


	Material sunMaterial;
	sunMaterial.setDiffuseMap("Assets/Demo/Planets/Sun/diffuse.jpg");
	sunMaterial.setSpecularMap("Assets/Demo/Planets/Sun/Specular.jpg");
	sunMaterial.isEmissive(true);
	Material moonMaterial;
	moonMaterial.setDiffuseMap("Assets/Demo/Planets/Moon/diffuse.jpg");
	moonMaterial.setSpecularMap("Assets/Demo/Planets/Moon/Specular.jpg");
	moonMaterial.isEmissive(false);
	Material earthMaterial;
	earthMaterial.setDiffuseMap("Assets/Demo/Planets/Earth/Earth map.jpg");
	earthMaterial.setSpecularMap("Assets/Demo/Planets/Earth/Specular map.jpg");
	earthMaterial.isEmissive(false);

	EntityID sun = scene->CreateEntity();
	scene->AddComponent<Mesh>(sun, Mesh());
	scene->AddComponent<Material>(sun, sunMaterial);
	scene->AddComponent<Transform>(sun, Transform(0, 0, 0));

	scene->GetComponent<Mesh>(sun).CreateMesh(generateSphereVertices(30, 30, 30));

	EntityID earth=scene->CreateEntity();

	scene->AddComponent<Mesh>(earth,Mesh());
	scene->AddComponent<Material>(earth,earthMaterial);
	scene->AddComponent<Transform>(earth,Transform(0,0,0));
	scene->AddComponent<BehaviourComponent>(earth,BehaviourComponent());

	scene->GetComponent<Mesh>(earth).CreateMesh(generateSphereVertices(30,30,6));
	scene->GetComponent<BehaviourComponent>(earth).setBehaviour<EarthBehv>(earth,sun,scene);

	EntityID moon = scene->CreateEntity();

	scene->AddComponent<Mesh>(moon, Mesh());
	scene->AddComponent<Material>(moon, moonMaterial);
	scene->AddComponent<Transform>(moon, Transform(0, 0, 0));
	scene->AddComponent<BehaviourComponent>(moon, BehaviourComponent());

	scene->GetComponent<Mesh>(moon).CreateMesh(generateSphereVertices(30, 30, 1));
	scene->GetComponent<BehaviourComponent>(moon).setBehaviour<MoonBehv>(moon,earth, scene);

	
	
	
	
	//scene->backGroundColor(0.1, 0.3, 0.2, 1);
	//scene->backGroundColor(0.2, 0.2, 0.2, 1);
	scene->backGroundColor(0, 0, 0, 1);

	while (!window.closeWindow())
	{
		
	
		scene->Render();
		
	}



	return 0;
}





