#pragma once


#include"Core/AkameCore.h"
class ECS;
/*
* This class handels the BehaviourComponent Attached to An entity
*/
class BehaviourSystem :public System
{
	std::weak_ptr<ECS> ecs;
	friend class Scene;
public:

	//is called when Scene::OnStart() is called
	AKAME_API void OnStart();

	//is called when Scene::Render is called.
	AKAME_API void Update(float deltaTime);

	//this is called everytime a BehviourComponent is added to an Entity.
	AKAME_API void OnAddEntity(Entity entity) override;

	
};