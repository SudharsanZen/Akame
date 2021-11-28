#pragma once
#include"ECS.h"
#include"Components/Components.h"
#include"SkeletalMesh.h"

class AnimationClip
{

};
class AnimationController : public Components
{
public:
	AnimationClip current;

	std::shared_ptr<std::vector<Bone>> boneList;
	std::shared_ptr<std::map<std::string,Bone>> boneMap;

	void reset() {}
};