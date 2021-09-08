#pragma once
#include<iostream>
#include<bitset>
#include<tuple>
using Entity = std::size_t;
using ComponentBitPosition = std::uint8_t;
using ComponentID = const char*;
using Signature = std::bitset<32>;

struct EntityID
{
private:
	Entity version;
	Entity index;

	friend class ECS;
	friend class Editor;
public:


	EntityID():EntityID(0,0)
	{
	}
	EntityID(Entity index,Entity version)
	{
		this->index = index;
		this->version = version;
		
	}

	bool operator == (EntityID const &e)const 
	{
		return std::tie(index, version) == std::tie(e.index, e.version);
	}

	bool operator != (EntityID const& e)const
	{
		return std::tie(index, version) != std::tie(e.index, e.version);
	}

	bool operator < (EntityID const& e)const
	{
		return std::tie(index,version)<std::tie(e.index,e.version);
	}


	bool operator > (EntityID const &e)const 
	{
		return std::tie(index, version) > std::tie(e.index, e.version);
	}

	friend std::ostream& operator <<(std::ostream& os, const EntityID& ID);
};

