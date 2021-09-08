#include"Components/Rendering/Transform.h"
#include"ECS.h"


class SceneTransformManager :public System
{
	std::weak_ptr<ECS> ecs;
	friend class Editor;
	friend class Scene;
public:

	
	void UpdateTransforms()
	{
		std::shared_ptr<ECS> e = ecs.lock();
		for (auto ent : entities)
		{
			Transform& t = e->GetComponent<Transform>(ent);
			if (t.parent == EntityID(-1,-1))
			{
				t.updateChildBaseTransformDetails();
			}
		}
	}

	//this is called everytime a BehviourComponent is added to an Entity.
	void OnAddEntity(Entity entity) override
	{

	}


};