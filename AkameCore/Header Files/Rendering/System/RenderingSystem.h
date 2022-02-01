#pragma once

#include"Math/GlmMath.h"
#include<memory>
#include<set>
#include"ECS.h"
#include"Rendering/FrameBuffer.h"
#include"Rendering/System/ShadowFBO.h"
#include"Rendering/System/TiledRenderer.h"
#include<map>
#include<unordered_map>
#include"Components/Animation/SkeletalMesh.h"
#include"Animation/SkeletalMeshRenderingSystem.h"
class Camera;
class LightSystem;
class RenderingSystem :public System
{
private:
	unsigned int transformUBO;
	int maxUBOSize,maxTransformUploadable;
	std::weak_ptr<SkeletalMeshRenderingSystem> animSys;
	ComponentArray<Transform>* transformArray;
	ComponentArray<Mesh>* meshArray;

	//map that holds list of entities under a particular shader
	std::unordered_map<std::string,std::map<unsigned long long,std::vector<Entity>>> drawList;

	
	
	//a weak pointer to the current entity component system that belongs to the current scene that houses this RenderingSystem
	std::weak_ptr<ECS> ecs;

	//uniform buffer that stores all the common values like view matrix, projection matrix and etc
	unsigned int uboMatrixBufferID;
	signed   long long int mat4Size = sizeof(glm::mat4);
	bool editorMode=false;
	bool m_update_Event;
	friend class Scene;
	friend class Editor;
	friend class LightAndShadowConfig;
	friend class Exporter;
	/*Call this to update the common global uniforms.
	* this updates the Projection matrix and ViewMatrix 
	*which is common to all shaders*/
	void updateUniformBuffer(Camera& cam);
	void GroupEntityWithCommonShader();
	/*
	* attach Shader Render pipeline class to the respective built in shaders
	*/
	void attachAllBuiltInSRP();
public:
	//fill the drawList map with relevant entities to their respective shaders
	bool EditorMode() { return editorMode; };
	std::weak_ptr<LightSystem> lightsystem;
	//window heigh and width to render to
	int height, width;
	//constructor
	RenderingSystem();
	//Call this to render A single frame
	void Run(Camera& cam, unsigned int frameBuffer=0);
	//clear the draw list
	void emptyDrawList();
	//this is a call back function which is called evertime a new entity is registered under rendering system
	void OnAddEntity(Entity entity) override 
	{
		m_update_Event = true;
	}
	//this is a call back function which is called when an entity is destroyed 
	void AfterDestroyEntity() override
	{ 
		m_update_Event = true;
	}
	//call this update the frame buffer's size to which Meshes are drawn to
	void updateFrameBufferSize(int height,int width);
	//change the viewPort's buffer size
	void viewPortSize(int x,int y,int height,int width);
	//Switch on depth test
	void depthTestOn(bool state);
	//Render all the entities under the given shader name
	void RenderAllEntitiesWithShader(std::string SHADERNAME,Camera cam, unsigned int frameBuffer=0);
	//Renders all the existing renderable entity under the given shader
	void RenderAllMesh(std::shared_ptr<Shader> shader,Camera cam);
	//Render all the entity under the given rendering queue
	void RenderQueue(std::string QUEUENAME,Camera cam, unsigned int frameBuffer=0);
};

