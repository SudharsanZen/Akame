#include"Scripting/ScriptableSystem.h"
#include"Components/Behaviour/ScriptComponent.h"
#include"ECS.h"
#include"Core/ScriptExporter.h"
#include"Core/Log/Log.h"
#include"Core/OS/OS_Interface.h"
AKAME_API void ScriptableSystem::set_watch_directory(std::string dir)
{
	ENGINE_CORE_INFO("visual studio project directory has been set to:"+dir);
	std::string dll_dir = dir+"\\bin\\Release\\x86_64\\Samples\\Scriptables.dll";
	m_watch_directory = dll_dir;
	reload = true;
}

void ScriptableSystem::OnStart()
{
	// get each entity that has this system's signature and  process it
	for (auto const& ent : entities)
	{
		ecs.lock()->GetComponent<ScriptComponent>(ent).OnStart(ecs);
	}
}

AKAME_API void ScriptableSystem::Update(float deltaTime)
{
	ReloadAll();
	// get each entity that has this system's signature and  process it
	for (auto const& ent : entities)
	{

		ecs.lock()->GetComponent<ScriptComponent>(ent).OnUpdate(ecs, deltaTime);

	}
}

AKAME_API void ScriptableSystem::OnAddEntity(Entity entity)
{
	auto& comp = ecs.lock()->GetComponent<ScriptComponent>(entity);
	comp.eid = entity;
	new_additions.push_back(entity);
}

inline AKAME_API void ScriptableSystem::update_new_additions()
{

	for (auto ent : new_additions)
	{
		auto scr_fact = ScriptObjectFactory::get_instance();
		auto& com = ecs.lock()->GetComponent<ScriptComponent>(ent);
		auto itr = scr_fact->m_object_gen_functor.find(com.comp_name);

		if (itr == scr_fact->m_object_gen_functor.end())
		{
			com.reset();
			ecs.lock()->RemoveComponent<ScriptComponent>(ent);
			ENGINE_CORE_ERROR("Can't find the script named: " + com.comp_name);
		}
		else
		{
			com.reset();
			std::shared_ptr<Behaviour> b = scr_fact->create_ptr(com.comp_name);
			com.setBehaviour(b);
		}
	}
	new_additions.clear();
}

AKAME_API void ScriptableSystem::ReloadAll()
{
	static HINSTANCE hinstLib = NULL;

	if (!reload)
		return;
	reload = false;
	std::filesystem::path dll_p = m_watch_directory;
	if (!std::filesystem::exists(dll_p))
		return;
	for (auto ent : entities)
	{
		auto& com = ecs.lock()->GetComponent<ScriptComponent>(ent);
		com.reset();
	}
	if (hinstLib == NULL)
	{
		hinstLib = LoadLibraryA(m_watch_directory.c_str());
	}
	else
	{
		if (!FreeLibrary(hinstLib))
		{
			ENGINE_CORE_CRITICAL("CAN'T FREE SCRIPTING.DLL!");
			return;
		}
		else
		{
			hinstLib = NULL;
			ENGINE_CORE_INFO("FREED PREVIOUS SCRIPTING.DLL!");
		}

		hinstLib = LoadLibraryA(m_watch_directory.c_str());
	}
	if (hinstLib == NULL)
	{
		ENGINE_CORE_ERROR("CAN'T LOAD THE SCIRPTING.DLL FILE!!");
		return;
	}
	for (auto ent : entities)
	{
		auto scr_fact = ScriptObjectFactory::get_instance();
		auto &com = ecs.lock()->GetComponent<ScriptComponent>(ent);
		auto itr = scr_fact->m_object_gen_functor.find(com.comp_name);

		if (itr == scr_fact->m_object_gen_functor.end())
		{
			ecs.lock()->RemoveComponent<ScriptComponent>(ent);
			ENGINE_CORE_ERROR("Can't find the script named: "+com.comp_name);
		}
		else
		{
			com.reset();
			std::shared_ptr<Behaviour> b = scr_fact->create_ptr(com.comp_name);
			com.setBehaviour(b);
		}
	}
}
