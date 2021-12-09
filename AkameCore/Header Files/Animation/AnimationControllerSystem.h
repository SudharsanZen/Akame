#pragma once
#include"ECS.h"
#include"Components/Animation/AnimationController.h"
#include"Core/Debug/Debug.h"
class AnimationControllerSystem :public System
{
	friend class Scene;
	std::weak_ptr<ECS> ecs;
	void update(float deltaTime)
	{
		std::shared_ptr<ECS> e = ecs.lock();

		//debug render bone pose
		for (auto ent : entities)
		{
			AnimationController& anim = e->GetComponent<AnimationController>(ent);
			for (int i = 0; i < anim.boneList->size(); i++)
			{
				Transform& curr = e->GetComponent<Transform>((*anim.boneList)[i].eid);
				glm::vec3 currPose = curr.GetGlobalPosition();
				Debug::DrawCircle(currPose, curr.up(), 0.02, glm::vec3(1, 0.5, 0));
				Debug::DrawCircle(currPose, curr.forward(), 0.02, glm::vec3(1, 0.5, 0));
				Debug::DrawCircle(currPose, curr.right(), 0.02, glm::vec3(1, 0.5, 0));

				Entity parent=curr.getParentId();
				if (parent != Entity(-1, -1))
				{
					Transform& pT = e->GetComponent<Transform>(parent);

					Debug::DrawLine(curr.GetGlobalPosition(),pT.GetGlobalPosition(),glm::vec3(0,1,0));
				}
			}

			anim.updateBoneState(deltaTime,e);
		}
	}

};