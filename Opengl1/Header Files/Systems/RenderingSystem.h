#pragma once

#include"GlmMath.h"
#include<memory>
#include<set>
#include"ECS.h"
#include"FrameBuffer.h"
#include"DeferredRendererFragmentBuffer.h"
class Camera;
class LightSystem;
class RenderingSystem:public System
{
private:

	FrameBuffer fb;
	DeferredRendererFragmentBuffer drfb;
	glm::vec3 lightPose;
	std::unordered_map<std::string, std::vector<Entity>> drawList;
	void GroupEntityWithCommonShader(std::shared_ptr<ECS> ecs);
	std::weak_ptr<LightSystem> lightsystem;

	friend class Scene;
	void setDeferredLightUniforms(std::shared_ptr<ECS> ecs,std::shared_ptr<Shader> shader, Camera& cam);
public:

	RenderingSystem()
	{
		lightPose = glm::vec3(10, 4, 10);
		emptyDrawList();
		
	}
	void Run(std::shared_ptr<ECS> ecs, Camera& cam);
	void emptyDrawList();

	void updateFrameBufferSize(int height,int width);

};

