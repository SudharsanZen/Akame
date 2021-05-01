#pragma once
#include"ECSTYPES.h"
std::ostream& operator <<(std::ostream& os, const EntityID& ID)
{
	os << unsigned(ID.version) << '-' << unsigned(ID.index);
	return os;
}