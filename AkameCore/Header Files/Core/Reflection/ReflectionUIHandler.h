#pragma once
#include<iostream>
#include<map>
#include<sstream>
#include"Math/GlmMath.h"
#include"Math/Quaternion.h"
#include<list>
#include <ECSTYPES.h>
#define AK_ID(id_name) (#id_name,&id_name,ID_TYPE::SIMPLE())
#define AK_ID_LIST(id_name) (#id_name,&id_name,ID_TYPE::SIMPLE_LIST())
#define AK_ID_COMPX(id_name) (#id_name,&id_name,ID_TYPE::COMPLEX())
#define AK_ID_COMPX_LIST(id_name) (#id_name,&id_name,ID_TYPE::COMPLEX_LIST())

#define AK_SERIALIZABLES(x)		void _serialize_data(ReflectionMeta& r){r x;}
#define AK_SHOW_IN_INSPECTOR(x) void _draw_data(ReflectionUIHandler& r){r x;}
namespace ID_TYPE
{
	class SIMPLE {};
	class COMPLEX {};
	class SIMPLE_LIST {};
	class COMPLEX_LIST {};
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
	
	template<typename T,typename _dummyType>
	ReflectionUIHandler& operator ()(std::string name, T* ptr,_dummyType type)
	{
		DrawTitle(name);
		DrawUIField(name,ptr);
	
		return *this;
	}
};

