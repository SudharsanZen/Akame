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
	AKAME_API SceneTransformManager();
	
	//update all the transforms and their child that needs an update
	AKAME_API void UpdateTransforms();
	AKAME_API void OnAddEntity(Entity entity) override;
	AKAME_API void OnDestroyEntity(Entity entity)override;

};