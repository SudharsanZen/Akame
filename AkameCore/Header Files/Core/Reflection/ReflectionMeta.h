#pragma once
#include<sstream>
#include"Math/GlmMath.h"
#include"Math/Quaternion.h"
#include<list>
#include <ECSTYPES.h>
#include<Core\Reflection\ReflectionUIHandler.h>
#include<json/single_include/nlohmann/json.hpp>

namespace json = nlohmann;
class AKAME_API ReflectionMeta
{
	bool _reading = false;
	bool _writing = false;

	template<typename T>
	void JsonToTypeCmpx(json::json& json, T* val)
	{
		ReflectionMeta meta;
		meta.m_json_object = json;
		meta.write(*val);
	}
	void JsonToTypeCmpx(json::json& json, Entity* val);

	void JsonToTypeCmpx(json::json& json, Quaternion* quat);
	void JsonToTypeCmpx(json::json& json, glm::vec3* vec3);
	void JsonToTypeCmpx(json::json& json, glm::vec4* vec4);
	void JsonToTypeCmpx(json::json& json, glm::quat* quat);


	void typeToJsonCmpx(json::json& json, Quaternion* quat);
	void typeToJsonCmpx(json::json& json, glm::vec3* vec3);
	void typeToJsonCmpx(json::json& json, glm::vec4* vec4);
	void typeToJsonCmpx(json::json& json, glm::quat* quat);
	template<typename T>
	void typeToJsonCmpx(json::json& json, T* ptr)
	{
		ReflectionMeta refMeta;
		(refMeta.read(*ptr));
		json = refMeta.m_json_object;
	}

	void typeToJsonCmpx(json::json& json, Entity* ptr);


public:

	json::json m_json_object;
	template<typename T>
	void read(T& from)
	{
		m_json_object.clear();
		_reading = true;
		from._serialize_data(*this);
		_reading = false;
	}
	template<typename T>
	void write(T& to)
	{
		_writing = true;
		to._serialize_data(*this);
		_writing = false;
	}
	std::string to_string();
	void print_formatted() { std::cout << m_json_object.dump(4) << std::endl; }

	void from_string(std::string searialized_class);

	template<typename T>
	ReflectionMeta& operator()(std::string name, T* ptr, ID_TYPE::SIMPLE _dummyType)
	{
		if (_reading)
		{
			m_json_object[name] = *ptr;
		}
		if (_writing)
		{

			auto itr = m_json_object.find(name);
			if (itr != m_json_object.end())
			{
				*ptr = m_json_object[name];
			}
		}
		return *this;
	}

	template<typename T>
	ReflectionMeta& operator()(std::string name, T* ptr, ID_TYPE::COMPLEX _dummyType)
	{
		if (_reading)
		{

			json::json obj;
			typeToJsonCmpx(obj, ptr);
			m_json_object[name] = obj;

		}
		if (_writing)
		{
			auto itr = m_json_object.find(name);
			if (itr != m_json_object.end())
			{
				JsonToTypeCmpx(m_json_object[name], ptr);
			}
		}
		return *this;
	}



	template<typename LIST>
	ReflectionMeta& operator ()(std::string name, LIST ptr, ID_TYPE::COMPLEX_LIST _dummyType)
	{

		if (_reading)
		{
			json::json arrayObj = json::json::array_t();

			for (auto& itr : *ptr)
			{
				json::json compObj;
				typeToJsonCmpx(compObj, &itr);
				arrayObj.push_back(compObj);
			}

			m_json_object[name] = arrayObj;
		}
		if (_writing)
		{
			if (m_json_object.find(name) == m_json_object.end())
				throw("can't find the array attrib in json object");
			ptr->clear();
			ptr->resize(m_json_object[name].size());
			size_t curr_i = 0;

			for (auto& itr : m_json_object[name])
			{
				JsonToTypeCmpx(itr, &(*ptr)[curr_i]);
				curr_i++;
			}
		}


		return *this;
	}

};

