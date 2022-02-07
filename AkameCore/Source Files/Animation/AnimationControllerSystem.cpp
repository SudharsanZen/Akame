#include"Animation/AnimationControllerSystem.h"
#include"ECS.h"
void AnimationControllerSystem::update(float deltaTime)
{

	std::shared_ptr<ECS> e = ecs.lock();

	for (auto ent : entities)
	{
		AnimationController& anim = e->GetComponent<AnimationController>(ent);
		anim.updateBoneState(deltaTime, e);
	}
}
