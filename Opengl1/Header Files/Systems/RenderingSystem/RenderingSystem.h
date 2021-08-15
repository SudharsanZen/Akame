#pragma once

#include"GlmMath.h"
#include<memory>
#include<set>
#include"ECS.h"
#include"FrameBuffer.h"
#include"Systems/RenderingSystem/ShadowFBO.h"
#include"Systems/RenderingSystem/TiledRenderer.h"
class Camera;
class LightSystem;
class RenderingSystem :public System
{
private:
	Mesh quad;

	FrameBuffer fb;
	TiledRenderer drfb;
	ShadowFBO dir_sMap;


	glm::vec3 lightPose;
	std::unordered_map<std::string, std::vector<Entity>> drawList;
	void GroupEntityWithCommonShader(std::shared_ptr<ECS> ecs);
	std::weak_ptr<LightSystem> lightsystem;
	std::weak_ptr<ECS> ecs;
	unsigned int uboMatrixBufferID;
	signed   long long int mat4Size = sizeof(glm::mat4);
	friend class Scene;

	/*Call this to update the common global uniforms.
	* this updates the Projection matrix and ViewMatrix which is common to all shaders
	*/
	void updateUniformBuffer(Camera& cam);


public:
	int height, width;
	RenderingSystem();
	void Run(Camera& cam);
	void emptyDrawList();
	void OnAddEntity(Entity entity) override {GroupEntityWithCommonShader(ecs.lock());}
	void OnDestroyEntity(Entity entity) override{ GroupEntityWithCommonShader(ecs.lock()); };
	void updateFrameBufferSize(int height,int width);

};

