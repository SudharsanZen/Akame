
#include"Engine.h"
#include"Editor/Scene.h"




class EarthBehv :public Behaviour
{
	float offx, offz;
	float y = 0;
	float sunEarthAngle = 0;
	float sunEarthDist =200;
	EntityID sun;
	
public:
	EarthBehv(EntityID sun)
	{
		
		this->sun = sun;
		offx = 0, offz = 0;
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

		GetComponent<Transform>().rotation.setEulerAngle(0,y,0);

		

		Quaternion rot = Quaternion::rotationAroundAxisVector(sunEarthAngle, worldUp);
		glm::vec3 sunPosition = GetComponent<Transform>(sun).position;

		GetComponent<Transform>().position = sunPosition + rot * ((glm::vec3(0, 0, 1)*sunEarthDist));//38.4400
	}
};
class MoonBehv :public Behaviour
{
	
	EntityID earth;
	float y = 0;
	float eartMoonAng=0;
	float distanceFromEarth =30;

	
public:
	MoonBehv(EntityID earth) 
	{
		this->earth = earth;
	}
	void OnStart()override
	{
		
	}

	void Update(float deltaTime)override
	{
		Transform& transform= GetComponent<Transform>(entityID);
		Transform& earthTransform= GetComponent<Transform>(earth);

		eartMoonAng += deltaTime * 100;
		y += 1 * deltaTime * 10;
		if (y > 360)
			y = 0;
		if (eartMoonAng > 360)
			eartMoonAng = 0;

		transform.rotation.setEulerAngle(0, y, 0);

		Quaternion rot=Quaternion::rotationAroundAxisVector(eartMoonAng,worldUp);
		glm::vec3 earthPosition=GetComponent<Transform>(earth).position;
		
		transform.position = earthPosition+rot*glm::vec3(0,0,1)*distanceFromEarth;
		
	}
};

int main()
{
	AssetManager::setAssetRoot("../../../Assets/");
	std::string rootDir(AssetManager::getAssetRoot());
	std::cout << AssetManager::getAssetRoot();
	Window window(800,800,"planets");
	
	if (!window.initialize())
	{
		std::cout << "Something went wrong, can't initialize window";
		return -1;
	}
	
	Scene scene(window);





	Material moonMaterial;
	moonMaterial.setTexture2D("material.diffuse", rootDir + "Media/Demo/Planets/Moon/diffuse.jpg");
	moonMaterial.setTexture2D("material.specular", rootDir + "Media/Demo/Planets/Moon/Specular.jpg");

	
	Material earthMaterial;
	earthMaterial.setTexture2D("material.diffuse",rootDir + "Media/Demo/Planets/Earth/Earth map.jpg");
	earthMaterial.setTexture2D("material.specular", rootDir + "Media/Demo/Planets/Earth/Specular map.jpg");

	Material sunMaterial;
	sunMaterial.setTexture2D("material.diffuse", rootDir + "Media/Demo/Planets/Sun/diffuse.jpg");
	sunMaterial.setTexture2D("material.specular", rootDir + "Media/Demo/Planets/Sun/Specular.jpg");
	sunMaterial.isEmissive(true);
	EntityID sun = scene.CreateEntity();

	//component that describes the shape
	scene.AddComponent<Mesh>(sun, Mesh());
	//component that describes the material
	scene.AddComponent<Material>(sun, sunMaterial);
	//component that describes the location ,size and rotation
	scene.AddComponent<Transform>(sun, Transform(0, 0, 0)); 
	//get the Mesh component and define it's shape, in this case a sphere
	scene.GetComponent<Mesh>(sun).CreateMesh(generateSphereVertices(30, 30, 30));

	EntityID earth=scene.CreateEntity();

	scene.AddComponent<Mesh>(earth,Mesh());
	scene.AddComponent<Material>(earth,earthMaterial);
	scene.AddComponent<Transform>(earth,Transform(0,0,0));
	scene.AddComponent<BehaviourComponent>(earth,BehaviourComponent());

	scene.GetComponent<Mesh>(earth).CreateMesh(generateSphereVertices(30,30,10));
	scene.GetComponent<BehaviourComponent>(earth).setBehaviour<EarthBehv>(sun);

	EntityID moon = scene.CreateEntity();

	scene.AddComponent<Mesh>(moon, Mesh());
	scene.AddComponent<Material>(moon, moonMaterial);
	scene.AddComponent<Transform>(moon, Transform(0, 0, 0));
	scene.AddComponent<BehaviourComponent>(moon, BehaviourComponent());


	scene.GetComponent<Mesh>(moon).CreateMesh(generateSphereVertices(30, 30, 3));
	scene.GetComponent<BehaviourComponent>(moon).setBehaviour<MoonBehv>(earth);


	scene.backGroundColor(0.2f, 0.4f, 0.2f, 1.0f);
	//scene.backGroundColor(0, 0, 0, 1);

	scene.OnStart();
	while (!window.closeWindow())
	{
		
		flyCam(scene.cam, scene.getDeltaTime());
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		scene.Render();
	}
	
	return 0;
}





