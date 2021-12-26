
#include"Core/Engine.h"
#include"Core/Scene.h"




class EarthBehv :public Behaviour
{
	float offx, offz;
	float y = 0;
	float sunEarthAngle = 0;
	float sunEarthDist =200;
	Entity sun;
	
public:
	EarthBehv(Entity sun)
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

		GetComponent<Transform>().SetGlobalRotation(Quaternion(0,y,0));

		

		Quaternion rot = Quaternion::rotationAroundAxisVector(sunEarthAngle, worldUp);
		glm::vec3 sunPosition = GetComponent<Transform>(sun).GetGlobalPosition();

		GetComponent<Transform>().SetGlobalPosition(sunPosition + rot * ((glm::vec3(0, 0, 1)*sunEarthDist)));//38.4400
	}
};
class MoonBehv :public Behaviour
{
	
	Entity earth;
	float y = 0;
	float eartMoonAng=0;
	float distanceFromEarth =30;

	
public:
	MoonBehv(Entity earth) 
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

		transform.SetGlobalRotation(Quaternion(0, y, 0));

		Quaternion rot=Quaternion::rotationAroundAxisVector(eartMoonAng,worldUp);
		glm::vec3 earthPosition=GetComponent<Transform>(earth).GetGlobalPosition();
		
		transform.SetGlobalPosition( earthPosition+rot*glm::vec3(0,0,1)*distanceFromEarth);
		
	}
};

int main()
{
	AssetManager::setAssetRoot("../../../../Assets/");
	std::string rootDir(AssetManager::getAssetRoot());
	std::cout << AssetManager::getAssetRoot();
	Window window(800,800,"planets");
	
	if (!window.initialize())
	{
		std::cout << "Something went wrong, can't initialize window";
		return -1;
	}
	
	Scene scene(window);





	Material moonMaterial("DEFERRED");
	moonMaterial.setTexture2D("material.diffuse", rootDir + "Media/Demo/Planets/Moon/diffuse.jpg");
	moonMaterial.setTexture2D("material.specular", rootDir + "Media/Demo/Planets/Moon/Specular.jpg");

	
	Material earthMaterial("DEFERRED");
	earthMaterial.setTexture2D("material.diffuse",rootDir + "Media/Demo/Planets/Earth/Earth map.jpg");
	earthMaterial.setTexture2D("material.specular", rootDir + "Media/Demo/Planets/Earth/Specular map.jpg");

	Material sunMaterial("DEFERRED");
	sunMaterial.setTexture2D("material.diffuse", rootDir + "Media/Demo/Planets/Sun/diffuse.jpg");
	sunMaterial.setTexture2D("material.specular", rootDir + "Media/Demo/Planets/Sun/Specular.jpg");
	sunMaterial.isEmissive(true);
	Entity sun = scene.CreateEntity();

	//component that describes the shape
	Mesh &sunM=scene.AddComponent<Mesh>(sun);
	//component that describes the material
	scene.AddComponent<Material>(sun)=sunMaterial;
	//component that describes the location ,size and rotation
	scene.AddComponent<Transform>(sun); 
	//get the Mesh component and define it's shape, in this case a sphere
	sunM.CreateMesh(generateSphereVertices(30, 30, 30));

	Entity earth=scene.CreateEntity();

	Mesh &earthM=scene.AddComponent<Mesh>(earth);
	scene.AddComponent<Material>(earth)=earthMaterial;
	scene.AddComponent<Transform>(earth);
	scene.AddComponent<BehaviourComponent>(earth).setBehaviour<EarthBehv>(sun);
	earthM.CreateMesh(generateSphereVertices(30,30,10));
	
	Entity moon = scene.CreateEntity();

	scene.AddComponent<Mesh>(moon).CreateMesh(generateSphereVertices(30, 30, 3));
	scene.AddComponent<Material>(moon)= moonMaterial;
	scene.AddComponent<Transform>(moon);
	scene.AddComponent<BehaviourComponent>(moon).setBehaviour<MoonBehv>(earth);



	scene.backGroundColor(0.2f, 0.4f, 0.2f, 1.0f);

	scene.OnStart();
	while (!window.closeWindow())
	{
		
		flyCam(scene.cam, scene.getDeltaTime());
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		scene.Render();
		scene.swapBuffers();
	}
	
	return 0;
}





