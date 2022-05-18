#include"Core/ScriptExporter.h"

ScriptObjectFactory* ScriptObjectFactory::get_instance()
{
	static ScriptObjectFactory* __inst = nullptr;
	if (__inst == nullptr)
		__inst = new ScriptObjectFactory();

	return __inst;
}

void ScriptObjectFactory::free_all()
{
	for (auto ptr : get_instance()->ptr_list)
		delete ptr;
	get_instance()->ptr_list.clear();
}

const char* ScriptObjectFactory::get_name_list()
{
	auto __inst = get_instance();
	__inst->_class_name_list = "";
	for (auto name_pair : __inst->m_object_gen_functor)
	{
		__inst->_class_name_list += name_pair.first + ",";
	}
	return __inst->_class_name_list.c_str();
}

std::vector<std::string> ScriptObjectFactory::get_name_list_vec()
{
	auto __inst = get_instance();
	__inst->_class_name_list = "";
	std::vector<std::string> __name_list;
	for (auto name_pair : __inst->m_object_gen_functor)
	{
		__name_list.push_back(name_pair.first);
	}
	return __name_list;
}

std::shared_ptr<Behaviour> ScriptObjectFactory::create_ptr(std::string class_name)
{
	std::shared_ptr<Behaviour> beh_ptr;
	ScriptObjectFactory* __inst = get_instance();
	if (__inst == nullptr)
	{
		beh_ptr = nullptr;
		return beh_ptr;
	}
	auto itr = __inst->m_object_gen_functor.find(class_name);
	if (itr != __inst->m_object_gen_functor.end())
	{
		beh_ptr.reset(itr->second());

	}
	else
	{
		beh_ptr=nullptr;
	}
}

ScriptObjectFactory::~ScriptObjectFactory()
{
	free_all();
}

