#include"ComponentArray.h"
#include"ComponentManager.h"
#include"ECS.h"
#include"system.h"
ECS ecs;
class position
{
public:
	int x, y, z;

	void reset()
	{
		x=0,y=0,z=0;
	}
};

class gravity
{
public:
	float g;
	gravity(float G=9)
	{
		g = G;
	}
	void reset()
	{
		g=0;
	}

};


class Physics:public System
{
	
public:
	
	void Update()
	{
		for (Entity entity : entities)
		{
			position& pose = ecs.GetComponent<position>(entity);
			pose.x+=ecs.GetComponent<gravity>(entity).g;
		
		}
	}

};
int main()
{
	std::uint32_t a=1;
	ComponentManager m;
	m.RegisterComponent<position>();
	ecs.RegisterComponent<position>();
	ecs.RegisterComponent<gravity>();
	std::shared_ptr<Physics> physics= ecs.RegisterSystem<Physics>();
	
	Signature sig;
	sig.set(ecs.GetComponentBitPose<position>());
	sig.set(ecs.GetComponentBitPose<gravity>());
	
	ecs.SetSystemSignature<Physics>(sig);

	std::vector<EntityID> entity;

	
	for (int i = 0; i < 10; i++)
	{
		entity.push_back(ecs.CreateEntity());

		ecs.AddComponent(entity[i], position());
		ecs.AddComponent(entity[i], gravity(rand()%8));
	}

	physics->Update();
	physics->Update();
	physics->Update();
	for (int i = 0; i < 10; i++)
	{
		
		
		
		auto& pos = ecs.GetComponent<position>(entity[i]);
		std::cout <<"entity:("<<i<<"):\n"<<pos.x<<" "<<pos.y<<" "<<pos.z<<"\n";

	}

	


	return 0;
}