#include"ComponentArray.h"
#include"ComponentManager.h"
#include"ECS.h"
#include"system.h"
ECS ecs;
class position
{
public:
	float x, y, z;

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
	

	


	return 0;
}