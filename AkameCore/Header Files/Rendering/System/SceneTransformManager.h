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
			if (t.parent == Entity(-1,-1))
			{
				t.updateChildBaseTransformDetails();
			}
		}
	}


	void OnAddEntity(Entity entity) override
	{

	}

	void OnDestroyEntity(Entity entity)override
	{
		Transform& t = ecs.lock()->GetComponent<Transform>(entity);
		t.removeParent();
		t.destroyChildren();
	}

};