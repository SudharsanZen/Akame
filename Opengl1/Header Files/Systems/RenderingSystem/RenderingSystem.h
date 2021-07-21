#pragma once

#include"GlmMath.h"
#include<memory>
#include<set>
#include"ECS.h"
#include"FrameBuffer.h"
#include"Systems/RenderingSystem/TiledRenderer.h"
class Camera;
class LightSystem;
class RenderingSystem:public System
{
private:

	FrameBuffer fb;
	TiledRenderer drfb;
	glm::vec3 lightPose;
	std::unordered_map<std::string, std::vector<Entity>> drawList;
	void GroupEntityWithCommonShader(std::shared_ptr<ECS> ecs);
	std::weak_ptr<LightSystem> lightsystem;
	unsigned int uboMatrixBufferID;
	signed   long long int mat4Size = sizeof(glm::mat4);
	friend class Scene;
	/*Call this to update the common global uniforms.
	* this updates the Projection matrix and ViewMatrix which is common to all shaders
	*/
	void updateUniformBuffer(Camera& cam);


public:

	RenderingSystem();
	void Run(std::shared_ptr<ECS> ecs, Camera& cam);
	void emptyDrawList();

	void updateFrameBufferSize(int height,int width);

};

