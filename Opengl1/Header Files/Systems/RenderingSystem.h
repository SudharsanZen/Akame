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
	std::unordered_map<std::string, std::vector<Entity>> drawList;
	void GroupEntityWithCommonShader(std::shared_ptr<ECS> ecs);
public:

	RenderingSystem()
	{
		lightPose = glm::vec3(10, 4, 10);
		emptyDrawList();
		
	}
	void Run(std::shared_ptr<ECS> ecs, Camera& cam);
	void emptyDrawList();
};

