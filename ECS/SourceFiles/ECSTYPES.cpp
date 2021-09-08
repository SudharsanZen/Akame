#pragma once
#include"ECSTYPES.h"
#include<tuple>
std::ostream& operator <<(std::ostream& os, const EntityID& ID)
{
	os << unsigned(ID.version) << '-' << unsigned(ID.index);
	return os;
}

/*
bool operator == (EntityID &el, EntityID &er)
{
	return (el.version == er.version && el.index == er.index);
}

bool operator != (EntityID &el, EntityID &er)
{
	return !(el.version == er.version && el.index == er.index);
}

bool operator < (EntityID& el, EntityID& er)
{
	return ((el.index > er.index) || (el.index == er.index && el.version > er.version));
}

bool operator > (EntityID &el, EntityID &er)
{
	return ((el.index > er.index) || (el.index == er.index && el.version > er.version));
}*/