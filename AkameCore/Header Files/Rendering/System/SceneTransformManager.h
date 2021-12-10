#include"Components/Rendering/Transform.h"
#include"ECS.h"


class SceneTransformManager :public System
{
	std::weak_ptr<ECS> ecs;

	//list of all entities that needs to be updated 
	std::shared_ptr<std::set<e_index>> updateTransformList;
	
	friend class Editor;
	friend class Scene;
public:
	SceneTransformManager()
	{
		updateTransformList = std::make_shared<std::set<e_index>>();
	}
	
	//update all the transforms and their child that needs an update
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
		
		/*the pointer for the updateTransformList 
		*is shared with all the entities that has a transform component
		* this enables the component's to add itself to the "to be updated list"
		*/
		t.transformUpdateList = updateTransformList;
	}

	void OnDestroyEntity(Entity entity)override
	{
		Transform& t = ecs.lock()->GetComponent<Transform>(entity);
		t.removeParent();
		t.destroyChildren();
	}

};