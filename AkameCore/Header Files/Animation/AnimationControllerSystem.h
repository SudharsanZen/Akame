#pragma once

#include"Components/Animation/AnimationController.h"
#include"Core/Debug/Debug.h"
class AnimationControllerSystem :public System
{
public:
	std::weak_ptr<ECS> ecs;
	void update(float deltaTime);

};