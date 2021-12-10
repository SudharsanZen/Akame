#pragma once
#include"ECS.h"
#include"Components/Components.h"
#include"SkeletalMesh.h"

class AnimationClip
{
private:
	template <typename _keyValueType>
	struct Key
	{
		double time;
		_keyValueType value;
		Key(double time,_keyValueType value)
		{
			this->time = time;
			this->value = value;
		}
	};
	struct AnimKeys
	{
		unsigned int pose_count=0;
		unsigned int rot_count=0;
		unsigned int scale_count=0;

		std::vector<Key<glm::vec3>> position;
		std::vector<Key<glm::quat>> rotation;
		std::vector<Key<glm::vec3>> scale;
	};

	unsigned int numChannels;
	std::string clipName="Clip";
	friend class Model;
	friend class AnimationController;
	//duration in ticks 
	double duration;
	double ticksPerSec;

	std::map<std::string, AnimKeys> boneNameKeysMap;

public:
	AnimationClip()
	{
		duration = 0;
		ticksPerSec = 0;
	}

};
class AnimationController : public Components
{
	AnimationClip currentClip;
	friend class AnimationControllerSystem;
	void updateBoneState(float deltaTime,std::shared_ptr<ECS> ecs)
	{
		
		currTime += deltaTime*1000.0f;
		for (auto& boneStates : bonesAnimStates)
		{
			//current bone's name
			std::string boneName = boneStates.first;
			//current bone's key state
			auto& keyState=boneStates.second;
			if (boneMap->find(boneName) == boneMap->end())
				continue;
				BoneInfo& bone = (*boneMap)[boneName];
			
				//std::cout << "\n" << boneName;
				auto& Keys = currentClip.boneNameKeysMap[boneName];

				Transform& bTrans = ecs->GetComponent<Transform>(bone.eid);
				
				//position animation
				if (keyState.i_currPose+1 < Keys.pose_count)
				{
					unsigned long long currPose = keyState.i_currPose;
					float timeGap = (Keys.position[currPose+1].time-Keys.position[currPose].time);
					float interTime = currTime-Keys.position[currPose].time;

					glm::vec3 currInterPose =glm::mix(Keys.position[currPose].value,Keys.position[currPose+1].value,interTime/timeGap);
					bTrans.SetLocalPosition(currInterPose);

					if (Keys.position[(keyState.i_currPose + 1)].time <= currTime)
					{
						keyState.i_currPose++;
					}
				}
				else if(keyState.i_currPose<Keys.pose_count)
				{
					bTrans.SetLocalPosition(Keys.position[keyState.i_currPose].value);
				}

				//rotation animation
				if (keyState.i_currRot + 1 < Keys.rot_count)
				{
					unsigned long long currPose = keyState.i_currRot;
					float timeGap = (Keys.rotation[currPose + 1].time - Keys.rotation[currPose].time);
					float interTime = currTime - Keys.position[currPose].time;

					glm::quat currInterRot = glm::mix(Keys.rotation[currPose].value, Keys.rotation[currPose + 1].value, (interTime / timeGap));
					bTrans.SetLocalRotation(currInterRot);

					if (Keys.rotation[(keyState.i_currRot + 1)].time<= currTime)
					{
						keyState.i_currRot++;
					}
				}
				else if (keyState.i_currRot < Keys.rot_count)
				{
					bTrans.SetLocalRotation(Keys.rotation[keyState.i_currRot].value);
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
public:
	struct AnimState
	{
		unsigned long long  i_currPose=0;
		unsigned long long  i_currRot=0;
		unsigned long long  i_currScale=0;

		void reset() 
		{
			i_currPose = 0;
			i_currRot = 0;
			i_currScale = 0;
		}
	};

	//current clip time details
	//current time
	float currTime = 0;
	//current clip length
	float clipDuration = 0;
	//current normalized time
	float normalizedTime = 0;

	double timePerTick=0;

	std::map<std::string, AnimState> bonesAnimStates;

	void setCurrentClip(AnimationClip clip)
	{
		currentClip = clip;
		for (auto& bonesPair : clip.boneNameKeysMap)
		{
			bonesAnimStates[bonesPair.first]= AnimState();
		}

		clipDuration = clip.duration;
		currTime = 0;
		normalizedTime = 0;
		timePerTick = 1/clip.ticksPerSec;
		
	}

	

	std::shared_ptr<std::vector<BoneInfo>> boneList;
	std::shared_ptr<std::map<std::string,BoneInfo>> boneMap;

	void reset() 
	{

	}
};