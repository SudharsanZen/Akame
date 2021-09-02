#pragma once
#include<iostream>
#include<bitset>
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
public:
	EntityID()
	{
		version = 0;
		index = 0;
	}
	EntityID(Entity index,Entity version=0)
	{
		this->index = index;
		this->version = version;
	}

	bool operator == (EntityID e)
	{
		return (version == e.version && index == e.index);
	}

	bool operator != (EntityID e)
	{
		return !(version == e.version && index == e.index);
	}
	friend std::ostream& operator <<(std::ostream& os, const EntityID& ID);
};

