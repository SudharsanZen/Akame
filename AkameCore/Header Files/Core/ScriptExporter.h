#pragma once
#define AKAME_SCRIPT __declspec(dllexport)
#include"Core/Engine.h"
template<typename T>
Behaviour* ObjecGeneratorFunction()
{
	return new T();
}

#define REGISTER_SCRIPT(_type)AKAME_SCRIPT bool dummy_reg_##_type = ScriptObjectFactory::register_class<##_type>(#_type);
class ScriptObjectFactory
{
	std::map<std::string, Behaviour*(*)()> m_object_gen_functor;
	std::set<Behaviour*> ptr_list;
	std::string _class_name_list;
	friend class ScriptableSystem;
public:
	AKAME_API static ScriptObjectFactory* get_instance();
	AKAME_API static void free_all();
	template<class _cls>
	static bool register_class(std::string cls_name);
	AKAME_API static const char* get_name_list();
	AKAME_API static std::vector<std::string> get_name_list_vec();
	AKAME_API static std::shared_ptr<Behaviour> create_ptr(std::string class_name);
	AKAME_API ~ScriptObjectFactory();
};

template<class _cls>
bool ScriptObjectFactory::register_class(std::string cls_name)
{
	get_instance()->m_object_gen_functor[cls_name] = ObjecGeneratorFunction<_cls>;
	return true;
}
