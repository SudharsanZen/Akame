#pragma once
#include"ECSTYPES.h"
#include<set>
class System
{

public:
	std::set<Entity> entities;
	virtual void OnAddEntity() {};
};