#pragma once

#include"GlmMath.h"
#include<memory>
#include<set>
#include"ECS.h"


class Camera;
class RenderingSystem:public System
{
private:
	glm::vec3 lightPose;
public:
	RenderingSystem()
	{
		lightPose = glm::vec3(10, 4, 10);
	}
	void Run(std::shared_ptr<ECS> ecs, Camera& cam);

};

