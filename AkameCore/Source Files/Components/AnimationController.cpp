#include"Components/Animation/AnimationController.h"
#include"ECS.h"
void AnimationController::updateBoneState(float deltaTime, std::shared_ptr<ECS> ecs)
{

	currTime += deltaTime * 1000.0f;
	for (auto& boneStates : bonesAnimStates)
	{
		//current bone's name
		std::string boneName = boneStates.first;
		//current bone's key state
		auto& keyState = boneStates.second;
		if (boneMap->find(boneName) == boneMap->end())
			continue;
		BoneInfo& bone = (*boneMap)[boneName];

		//std::cout << "\n" << boneName;
		auto& Keys = currentClip.boneNameKeysMap[boneName];

		Transform& bTrans = ecs->GetComponent<Transform>(bone.eid);
		float minVal = 0;
		//position animation
		if (keyState.i_currPose + 1 < Keys.pose_count)
		{
			unsigned long long currPose = keyState.i_currPose;
			float timeGap = (Keys.position[currPose + 1].time - Keys.position[currPose].time);
			float interTime = currTime - Keys.position[currPose].time;

			interTime = glm::clamp(interTime, minVal, timeGap);//avoid extrapolation
			glm::vec3 currInterPose = glm::mix(Keys.position[currPose].value, Keys.position[currPose + 1].value, interTime / timeGap);
			bTrans.SetLocalPosition(currInterPose);

			if (Keys.position[(keyState.i_currPose + 1)].time <= currTime)
			{
				keyState.i_currPose++;
			}
		}
		else if (keyState.i_currPose<Keys.pose_count)
		{
			bTrans.SetLocalPosition(Keys.position[keyState.i_currPose].value);
		}

		//rotation animation
		if (keyState.i_currRot + 1 < Keys.rot_count)
		{
			unsigned long long currPose = keyState.i_currRot;
			float timeGap = (Keys.rotation[currPose + 1].time - Keys.rotation[currPose].time);
			float interTime = currTime - Keys.rotation[currPose].time;
			interTime = glm::clamp(interTime, minVal, timeGap);//avoid extrapolation
			glm::quat currInterRot = glm::slerp(Keys.rotation[currPose].value, Keys.rotation[currPose + 1].value, (interTime / timeGap));
			bTrans.SetLocalRotation(currInterRot);

			if (Keys.rotation[(keyState.i_currRot + 1)].time <= currTime)
			{
				keyState.i_currRot++;
			}
		}
		else if (keyState.i_currRot < Keys.rot_count)
		{
			bTrans.SetLocalRotation(Keys.rotation[keyState.i_currRot].value);
		}

		//scale animation
		if (keyState.i_currRot + 1 < Keys.scale_count)
		{
			unsigned long long currPose = keyState.i_currScale;
			float timeGap = (Keys.scale[currPose + 1].time - Keys.scale[currPose].time);
			float interTime = currTime - Keys.scale[currPose].time;
			interTime = glm::clamp(interTime, minVal, timeGap);//avoid extrapolation
			glm::vec3 currInterScale = glm::mix(Keys.scale[currPose].value, Keys.scale[currPose + 1].value, (interTime / timeGap));
			bTrans.SetLocalScale(currInterScale);

			if (Keys.scale[(keyState.i_currScale + 1)].time <= currTime)
			{
				keyState.i_currScale++;
			}
		}
		else if (keyState.i_currScale < Keys.scale_count)
		{
			bTrans.SetLocalScale(Keys.scale[keyState.i_currScale].value);
		}

	}
	if (currTime > clipDuration)
	{
		for (auto& keyState : bonesAnimStates)
		{
			keyState.second.reset();
		}
		currTime = 0;


	}
}

void AnimationController::setCurrentClip(AnimationClip clip)
{
	currentClip = clip;
	for (auto& bonesPair : clip.boneNameKeysMap)
	{
		bonesAnimStates[bonesPair.first] = AnimState();
	}

	clipDuration = clip.duration;
	currTime = 0;
	normalizedTime = 0;
	timePerTick = 1 / clip.ticksPerSec;

}

void AnimationController::reset()
{

}

void AnimationController::AnimState::reset()
{
	i_currPose = 0;
	i_currRot = 0;
	i_currScale = 0;
}

AnimationClip::AnimationClip()
{
	duration = 0;
	ticksPerSec = 0;
}
