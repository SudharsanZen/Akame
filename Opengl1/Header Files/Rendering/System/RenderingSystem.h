#pragma once

#include"Math/GlmMath.h"
#include<memory>
#include<set>
#include"ECS.h"
#include"Rendering/FrameBuffer.h"
#include"Rendering/System/ShadowFBO.h"
#include"Rendering/System/TiledRenderer.h"

class Camera;
class LightSystem;
class RenderingSystem :public System
{
private:
	

	//map that holds list of entities under a particular shader
	std::unordered_map<std::string, std::vector<Entity>> drawList;

	//fill the drawList map with relevant entities to their respective shaders
	void GroupEntityWithCommonShader(std::shared_ptr<ECS> ecs);
	
	//a weak pointer to the current entity component system that belongs to the current scene that houses this RenderingSystem
	std::weak_ptr<ECS> ecs;

	//uniform buffer that stores all the common values like view matrix, projection matrix and etc
	unsigned int uboMatrixBufferID;
	signed   long long int mat4Size = sizeof(glm::mat4);
	
	
	friend class Scene;

	/*Call this to update the common global uniforms.
	* this updates the Projection matrix and ViewMatrix 
	*which is common to all shaders*/
	void updateUniformBuffer(Camera& cam);

	/*
	* attach Shader Render pipeline class to the respective built in shaders
	*/
	void attachAllBuiltInSRP();
public:
	std::weak_ptr<LightSystem> lightsystem;
	//window heigh and width to render to
	int height, width;
	//constructor
	RenderingSystem();
	//Call this to render A single frame
	void Run(Camera& cam);
	//clear the draw list
	void emptyDrawList();
	//this is a call back function which is called evertime a new entity is registered under rendering system
	void OnAddEntity(Entity entity) override {GroupEntityWithCommonShader(ecs.lock());}
	//this is a call back function which is called when an entity is destroyed 
	void OnDestroyEntity(Entity entity) override{ GroupEntityWithCommonShader(ecs.lock()); };
	//call this update the frame buffer's size to which stuff are drawn too
	void updateFrameBufferSize(int height,int width);
	//change the viewPort's buffer size
	void viewPortSize(int x,int y,int height,int width);
	//Switch on depth test
	void depthTestOn(bool state);
	//Render all the entities under the given shader name
	void RenderAllEntitiesWithShader(std::string SHADERNAME,Camera cam);
	//Renders all the existing renderable entity under the given shader
	void RenderAllMesh(std::shared_ptr<Shader> shader,Camera cam);
	//Render all the entity under the given rendering queue
	void RenderQueue(std::string QUEUENAME,Camera cam);
};

