#pragma once
#include"Components/Components.h"
#include"SkeletalMesh.h"
#include"Core/AkameCore.h"
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
	AKAME_API AnimationClip();

};
class AnimationController : public Components
{
	AnimationClip currentClip;
	friend class AnimationControllerSystem;
	AKAME_API void updateBoneState(float deltaTime, std::shared_ptr<ECS> ecs);
public:
	struct AnimState
	{
		unsigned long long  i_currPose=0;
		unsigned long long  i_currRot=0;
		unsigned long long  i_currScale=0;

		AKAME_API void reset();
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

	AKAME_API void setCurrentClip(AnimationClip clip);

	

	std::shared_ptr<std::vector<BoneInfo>> boneList;
	std::shared_ptr<std::map<std::string,BoneInfo>> boneMap;

	AKAME_API void reset();
};