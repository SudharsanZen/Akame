#pragma once
#include<iostream>
#include<bitset>
using Entity = std::uint32_t;
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
	
	friend std::ostream& operator <<(std::ostream& os, const EntityID& ID);
};

