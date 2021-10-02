#include"Components/Rendering/Transform.h"
#include"ECS.h"


class SceneTransformManager :public System
{
	std::weak_ptr<ECS> ecs;

	std::shared_ptr<std::set<e_index>> updateTransformList;
	
	friend class Editor;
	friend class Scene;
public:
	SceneTransformManager()
	{
		updateTransformList = std::make_shared<std::set<e_index>>();
	}
	
	void UpdateTransforms()
	{
		
		std::shared_ptr<ECS> e = ecs.lock();
		e_index transformIndex = e->componentManager.compIDtoBitPose[typeid(Transform).name()];
		for (auto ent : *updateTransformList)
		{
			Transform& t = e->GetComponent<Transform>(ent,transformIndex);
			//if (t.parent == Entity(-1,-1))
			{
				t.updateChildBaseTransformDetails();
			}
		}
		updateTransformList->clear();
	}


	void OnAddEntity(Entity entity) override
	{
		const ComponentBitPosition TransformBitPose = ecs.lock()->GetComponentBitPose<Transform>();
		e_index transformComponentIndex = (*entity.componentIndex)[TransformBitPose];
		updateTransformList->insert(transformComponentIndex);
		Transform& t = ecs.lock()->GetComponent<Transform>(transformComponentIndex,TransformBitPose);
		t.transformUpdateList = updateTransformList;
	}

	void OnDestroyEntity(Entity entity)override
	{
		Transform& t = ecs.lock()->GetComponent<Transform>(entity);
		t.removeParent();
		t.destroyChildren();
	}

};