#pragma once
#include<iostream>
#include<bitset>
#include<tuple>
#include<vector>

using ComponentBitPosition = std::uint8_t;
using ComponentID = const char*;
using Signature = std::bitset<32>;
using  e_index = uint64_t;
#define ECS_INVALID_INDEX (e_index)(-1)
struct Entity
{
private:
	std::shared_ptr<bool> entityAlive;
	e_index version;
	e_index index;
	
	//attached components signature
	std::shared_ptr<Signature> signature;
	//actual index of the component in the Component array of ECS
	std::shared_ptr<std::vector<e_index>> componentIndex;

	template<typename T>
	friend class ComponentArray;

	friend class SystemManager;
	friend class RenderingSystem;
	friend class SceneTransformManager;
	friend class SceneHierarchyWindow;
	friend class Transform;
	friend class ECS;
	friend class Editor;
	friend class ReflectionMeta;
	friend class InspectorWindow;
public:
	
	e_index GetVersion() { return version; }
	e_index GetIndex() { return index; }


	Entity():Entity(ECS_INVALID_INDEX,ECS_INVALID_INDEX)
	{
	}
	Entity(e_index index,e_index version)
	{
		this->index = index;
		this->version = version;

		entityAlive = std::make_shared<bool>(false);
		
	}

	bool operator == (Entity const &e)const 
	{
		return std::tie(index, version) == std::tie(e.index, e.version);
	}

	bool operator != (Entity const& e)const
	{
		return std::tie(index, version) != std::tie(e.index, e.version);
	}

	bool operator < (Entity const& e)const
	{
		return std::tie(index,version)<std::tie(e.index,e.version);
	}


	bool operator > (Entity const &e)const 
	{
		return std::tie(index, version) > std::tie(e.index, e.version);
	}

	friend std::ostream& operator <<(std::ostream& os, const Entity& ID);
};


#define INVALID_ENTITY Entity(-1,-1)