
#include"ExporterUI.h"
/*A command line tool for converting well known 3d file formats to engine format*/
/*
int main()
{
	AssetManager::assetRootPath = "../../../../Assets/";
	//need to write code lol
	Window window(800,800,"Akame Exporter");
	if (!window.initialize())
	{
		throw("can't initialize window");
	}
	
	Scene scene(window);
	
	auto dir = scene.CreateEntity();
	scene.SetEntityName(dir,"Directional Light");
	scene.AddComponent<Transform>(dir);
	Lights d_light(LIGHT::DIRECTIONAL);
	d_light.setDirection(-90,0,0);
	scene.AddComponent<Lights>(dir) = d_light;



	Material mat("GRIDS");
	Entity pl = scene.CreateEntity();


	scene.AddComponent<Transform>(pl);
	Mesh& plm = scene.AddComponent<Mesh>(pl);
	scene.AddComponent<Material>(pl) = mat;
	plm.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);


	Exporter exp(scene);
	scene.vsyncOn(true);
	//Editor edt(scene);
	scene.backGroundColor(0,0,0,1);
	while (!window.closeWindow())
	{
		scene.clearBuffer();
			
			//flyCam(scene.cam,scene.getDeltaTime());
			//scene.Render();
			exp.DrawUI();
		
		scene.swapBuffers();
	}
	return 0;
}

*/
class EntityID
{
public:
	e_index index;
	e_index version;
	AK_SERIALIZABLES
	(
		AK_ID(index)
		AK_ID(version)
	)
};
class TransformComp
{
public:
	glm::vec3 pose;
	glm::quat rotation;
	glm::vec3 scale;
	EntityID eid = {0,1};
	std::vector<EntityID> eid_list;
	AK_SERIALIZABLES
	(
		AK_ID(pose)
		AK_ID(rotation)
		AK_ID(scale)
		AK_ID_COMPX(eid)
		AK_ID_COMPX_LIST(eid_list)
	)

};

int main()
{
	TransformComp t1 = { {0,1,0},{0,0,0,1},{1,1,1}};
	t1.eid_list = { {1,1},{2,1},{3,4} };
	TransformComp t2;
	ReflectionMeta meta1;
	ReflectionMeta meta2;
	
	meta1.read(t1);
	
	meta1.write(t2);
	std::string str = meta1.to_string();
	int i = 0;

	
	auto space = [](int a) {for (int i = 0; i < a; i++) std::cout << "\t"; };
	int tab = 0;
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '{')
		{
			tab+=1;

			std::cout << "\n";
			space(tab-1);
			std::cout << str[i];
			std::cout <<"\n";
			space(tab);
			
		}
		else if (str[i] == '}')
		{
			tab-=1;
			std::cout <<"\n";
			space(tab);
			std::cout << str[i];
			std::cout << "\n";
			space(tab);
			
		}
		else if (str[i] == '[')
		{
			std::cout << "\n";
			space(tab);
			//std::cout <<str[i];
			
			//std::cout << "\n";
			//space(tab);
		}
		else if(str[i]!=']')
		{
			std::cout<<str[i];
		}
	}
	std::cout <<std::endl<< meta1.to_string();
	return 0;
}



