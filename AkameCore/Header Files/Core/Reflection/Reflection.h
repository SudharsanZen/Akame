#pragma once
#include<iostream>
#include<map>
#include<sstream>
#include"Math/GlmMath.h"
#include"Math/Quaternion.h"
#include<list>
#define AK_ID(id_name) (#id_name,&id_name)
#define AK_ID_LIST(id_name) (#id_name,&id_name,false,true)
#define AK_ID_COMPX(id_name) (#id_name,&id_name,true)
#define AK_ID_COMPX_LIST(id_name) (#id_name,&id_name,true,true)

#define AK_SERIALIZABLES(x)		void _serialize_data(ReflectionMeta& r){r x;}
#define AK_SHOW_IN_INSPECTOR(x) void _draw_data(ReflectionUIHandler& r){r x;}


class ReflectionMeta
{
	bool _reading=false;
	bool _writing=false;
	template<typename T>
	void stringToType(std::string str_val, T* val)
	{
		std::stringstream ss;
		ss << str_val;
		ss >> *val;
	}

	void stringToType(std::string str_val, Quaternion* val);
	void stringToType(std::string str_val, glm::vec3* val);
	void stringToType(std::string str_val, glm::vec4* val);
	void stringToType(std::string str_val, glm::quat* val);

	template<typename T>
	void stringToTypeCmpx(std::string str_val, T* val)
	{
		ReflectionMeta meta;
		meta.from_string(str_val);
		meta.write(*val);
	}
	
	template<typename T>
	void typeToString(std::string name,T* ptr)
	{
		std::stringstream ss;
		ss << *ptr;
		m_id_value_pair[name] = ss.str();
	}

	void typeToString(std::string str_name, Quaternion *quat);
	void typeToString(std::string str_name, glm::vec3 *vec3);
	void typeToString(std::string str_name, glm::vec4 *vec4);
	void typeToString(std::string str_name,glm::quat *quat);
	template<typename T>
	void typeToStringCmpx(std::string name,T* ptr)
	{
		ReflectionMeta refMeta;
		(refMeta.read(*ptr));
		m_id_value_pair[name] =refMeta.to_string();
	}


public:

	std::map<std::string, std::string> m_id_value_pair;
	template<typename T>
	void read(T& from)
	{
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
	std::string to_string()
	{
		std::string str = "{";
		for (auto p : m_id_value_pair)
		{
			str += "[" + p.first + ":" + p.second + "]";
		}
		str += "}";
		return str;
	}

	std::string to_string_formated()
	{
		std::string str = "{\n";
		for (auto p : m_id_value_pair)
		{
			str += "[" + p.first + ":" + p.second + "]\n";
		}
		str += "}\n";
		return str;
	}

	void from_string(std::string searialized_class);
	template<typename T>
	ReflectionMeta& operator ()(std::string name, T* ptr)
	{
		if (_reading)
		{
			typeToString(name,ptr);
		}
		if (_writing)
		{
			auto itr = m_id_value_pair.find(name);
			if (itr != m_id_value_pair.end())
			{
				stringToType(itr->second, ptr);
			}
		}
		return *this;
	}

	template<typename T>
	ReflectionMeta& operator ()(std::string name, T* ptr, bool isComp)
	{
		if (_reading)
		{
			if (isComp)
				typeToStringCmpx(name, ptr);
		}
		if (_writing)
		{
			auto itr = m_id_value_pair.find(name);
			if (itr != m_id_value_pair.end())
			{
				stringToTypeCmpx(itr->second, ptr);
			}
		}
		return *this;
	}

	template<typename LIST>
	ReflectionMeta& operator ()(std::string name,LIST ptr, bool isComp,bool isList)
	{
		std::stringstream ss;
		if (_reading)
		{
			ss << ptr->size();
			m_id_value_pair[name + ".size"] = ss.str();
		}
		if (_writing)
		{
			size_t size;
			ss << m_id_value_pair[name + ".size"];
			ss >> size;
			ptr->clear();
			ptr->resize(size);
		}
		size_t ind = 0;
		if(isList)
			for (auto &itr :*ptr)
			{
				std::stringstream arrName;
				arrName << name << "(" << ind << ")";
				(*this) (arrName.str(),&itr,isComp);
				ind++;
			}
		return *this;
	}

};

class ReflectionUIHandler
{
	void DrawTitle(std::string name);
	void DrawUIField(std::string name, int *val);
	void DrawUIField(std::string name, float *val);
	void DrawUIField(std::string name, double *val);
	void DrawUIField(std::string name, glm::vec3* val);
	void DrawUIField(std::string name, glm::vec4* val);
	void DrawUIField(std::string name, bool *val);
	void DrawUIField(std::string name, Quaternion *val);
	void DrawUIField(std::string name, glm::quat *val);
	void DrawUIField(std::string name, std::string *val);
public:
	
	template<typename T>
	ReflectionUIHandler& operator ()(std::string name, T* ptr)
	{
		DrawTitle(name);
		DrawUIField(name,ptr);
	
		return *this;
	}
};

