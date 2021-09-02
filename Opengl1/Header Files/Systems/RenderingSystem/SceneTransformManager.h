#include"Transform.h"
#include"ECS.h"


class SceneTransformManager :public System
{
	std::weak_ptr<ECS> ecs;
	friend class Scene;
public:

	
	void UpdateTransforms()
	{

	}

	//this is called everytime a BehviourComponent is added to an Entity.
	void OnAddEntity(Entity entity) override
	{

	}


};