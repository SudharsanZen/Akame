#pragma once
#include"Editor/Scene.h"
class Behaviour
{
protected:

	EntityID entityID;
	
public:

	
	Behaviour(EntityID eid) :entityID(eid){};
	virtual ~Behaviour() = default;
	virtual void OnStart() = 0;
	virtual void Update(float deltaTime) = 0;
};
