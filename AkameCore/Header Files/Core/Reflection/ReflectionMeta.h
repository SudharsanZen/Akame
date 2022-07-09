#pragma once
#include<sstream>
#include"Math/GlmMath.h"
#include"Math/Quaternion.h"
#include<list>
#include <ECSTYPES.h>
#include<Core\Reflection\ReflectionUIHandler.h>
#include<json/single_include/nlohmann/json.hpp>

class SceneDeserializer;
class ReflectionMeta
{
	friend class ModelExporter;
	friend class SceneDeserializer;
	SceneDeserializer *m_deser;
	bool _reading = false;
	bool _writing = false;

	template<typename T>
	void JsonToTypeCmpx(nlohmann::json& json, T* val)
	{
		ReflectionMeta meta;
		meta.m_json_object = json;
		meta.write(*val);
	}
	AKAME_API void JsonToTypeCmpx(nlohmann::json& json, Entity* val);

	AKAME_API void JsonToTypeCmpx(nlohmann::json& json, Quaternion* quat);
	AKAME_API void JsonToTypeCmpx(nlohmann::json& json, glm::vec3* vec3);
	AKAME_API void JsonToTypeCmpx(nlohmann::json& json, glm::vec4* vec4);
	AKAME_API void JsonToTypeCmpx(nlohmann::json& json, glm::quat* quat);
	AKAME_API void JsonToTypeCmpx(nlohmann::json& json, glm::mat4* mat);


	AKAME_API void typeToJsonCmpx(nlohmann::json& json, Quaternion* quat);
	AKAME_API void typeToJsonCmpx(nlohmann::json& json, glm::vec3* vec3);
	AKAME_API void typeToJsonCmpx(nlohmann::json& json, glm::vec4* vec4);
	AKAME_API void typeToJsonCmpx(nlohmann::json& json, glm::quat* quat);
	AKAME_API void typeToJsonCmpx(nlohmann::json& json, glm::mat4* mat);
	template<typename T>
	void typeToJsonCmpx(nlohmann::json& json, T* ptr)
	{
		ReflectionMeta refMeta;
		(refMeta.read(*ptr));
		json = refMeta.m_json_object;
	}

	AKAME_API void typeToJsonCmpx(nlohmann::json& json, Entity* ptr);


public:
	ReflectionMeta();
	ReflectionMeta(SceneDeserializer* deserializer);
	nlohmann::json m_json_object;
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
	AKAME_API std::string to_string();
	AKAME_API void print_formatted();

	AKAME_API void from_string(std::string searialized_class);

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

			nlohmann::json obj;
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
			nlohmann::json arrayObj = nlohmann::json::array_t();

			for (auto& itr : *ptr)
			{
				nlohmann::json compObj;
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
				JsonToTypeCmpx(itr, &((*ptr)[curr_i]));
				curr_i++;
			}
		}


		return *this;
	}

};

