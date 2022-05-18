#pragma once
#include"System.h"
#include"Core/AkameCore.h"
#include<filesystem>


class ScriptableSystem :public System
{
	std::weak_ptr<ECS> ecs;
	std::vector<Entity> new_additions;
	friend class Scene;
	friend class InspectorWindow;
	std::string m_watch_directory;
	bool reload=false;
	
public:
	AKAME_API void set_watch_directory(std::string dir);
	//is called when Scene::OnStart() is called
	AKAME_API void OnStart();

	//is called when Scene::Render is called.
	AKAME_API void Update(float deltaTime);

	//this is called everytime a BehviourComponent is added to an Entity.
	AKAME_API void OnAddEntity(Entity entity) override;

	AKAME_API void update_new_additions();
	AKAME_API void ReloadAll();


};