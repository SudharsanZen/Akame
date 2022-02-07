#pragma once
#include"ECS.h"
#include"Components/Animation/AnimationController.h"
#include"Core/Debug/Debug.h"
class AKAME_API AnimationControllerSystem :public System
{
public:
	std::weak_ptr<ECS> ecs;
	void update(float deltaTime)
	{
		
		std::shared_ptr<ECS> e = ecs.lock();
		
		for (auto ent : entities)
		{
			AnimationController& anim = e->GetComponent<AnimationController>(ent);
			anim.updateBoneState(deltaTime,e);
		}
	}

};