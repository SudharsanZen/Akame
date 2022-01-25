#pragma once
#include<iostream>
#include<map>
#include<sstream>
#include"Math/GlmMath.h"
#include"Math/Quaternion.h"
#define AK_ID(id_name) (#id_name,&id_name)

#define AK_SERIALIZABLES(x)		void _serialize_data(ReflectionMeta& r){r x;}
#define AK_SHOW_IN_INSPECTOR(x) void _draw_data(ReflectionUIHandler& r){r x;}


class ReflectionMeta
{
	bool _reading;
	bool _writing;
	template<typename T>
	void stringToType(std::string str_val, T* val)
	{
		std::stringstream ss;
		ss << str_val;
		ss >> *val;
	}

	//void stringToType(std::string str_val, Quaternion* val);
	//void stringToType(std::string str_val, glm::vec3* val);
	//void stringToType(std::string str_val, glm::vec4* val);
	//void stringToType(std::string str_val, glm::quat* val);
	


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
			str += "[" + p.first + ":" + p.second + "],";
		}
		str += "}";
		return str;
	}
	template<typename T>
	ReflectionMeta& operator ()(std::string name, T* ptr)
	{
		if (_reading)
		{
			std::stringstream ss;
			ss << *ptr;
			m_id_value_pair[name] = ss.str();
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

