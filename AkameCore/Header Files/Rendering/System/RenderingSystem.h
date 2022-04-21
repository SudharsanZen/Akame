#pragma once

#include"Math/GlmMath.h"
#include<memory>
#include<set>
#include"Rendering/FrameBuffer.h"
#include"Rendering/System/ShadowFBO.h"
#include"Rendering/System/TiledRenderer.h"
#include<map>
#include<unordered_map>
#include"Components/Animation/SkeletalMesh.h"
#include"Math/Octree.h"
#include"Animation/SkeletalMeshRenderingSystem.h"
class Camera;
class LightSystem;
class ECS;
#include"ECSTYPES.h"
class RenderingSystem :public System
{
private:
	unsigned int transformUBO;
	int maxUBOSize,maxTransformUploadable;
	std::weak_ptr<SkeletalMeshRenderingSystem> animSys;
	ComponentArray<Transform>* transformArray;
	ComponentArray<Mesh>* meshArray;
	using DrawList = std::unordered_map<std::string, std::unordered_map<unsigned long long, std::vector<const Entity*>>>;
	//map that holds list of entities under a particular shader
	DrawList dynamic_drawList;
	DrawList static_drawList;
	DrawList drawList;

	
	std::vector<const Entity*> empty;
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
	AKAME_API void updateUniformBuffer(Camera& cam);
	AKAME_API void GroupEntityWithCommonShader();
	AKAME_API auto get_static_list(std::string shader_name,unsigned long long material_id);
	AKAME_API auto get_dynamic_list(std::string shader_name,unsigned long long material_id);
	template<typename _list>
	AKAME_API void get_sorted_render_list(DrawList& d_list,_list &entity_list);
	/*
	* attach Shader Render pipeline class to the respective built in shaders
	*/
	AKAME_API void attachAllBuiltInSRP();
public:
	float time_taken;
	std::shared_ptr<OctTree> static_tree;
	//fill the drawList map with relevant entities to their respective shaders
	AKAME_API bool EditorMode();
	std::weak_ptr<LightSystem> lightsystem;
	//window heigh and width to render to
	int height, width;
	//constructor
	AKAME_API RenderingSystem(Scene& scene);
	//Call this to render A single frame
	AKAME_API void Run(Camera& cam, unsigned int frameBuffer=0);
	//clear the draw list
	AKAME_API void emptyDrawList();
	AKAME_API void emptyStaticDrawList();
	//this is a call back function which is called evertime a new entity is registered under rendering system
	AKAME_API void OnAddEntity(Entity entity) override;
	//this is a call back function which is called when an entity is destroyed 
	AKAME_API void AfterDestroyEntity() override;
	//call this update the frame buffer's size to which Meshes are drawn to
	AKAME_API void updateFrameBufferSize(int height,int width);
	//change the viewPort's buffer size
	AKAME_API void viewPortSize(int x,int y,int height,int width);
	//Switch on depth test
	AKAME_API void depthTestOn(bool state);
	//Render all the entities under the given shader name
	AKAME_API void RenderAllEntitiesWithShader(std::string SHADERNAME,Camera cam, unsigned int frameBuffer=0);
	//Render all the entities under the given shader name for PSSM
	AKAME_API void RenderAllEntitiesPSSM(std::shared_ptr<Shader> shader,Camera cam);
	//Renders all the existing renderable entity under the given shader
	AKAME_API void RenderAllMesh(std::shared_ptr<Shader> shader,Camera cam);
	//Render all the entity under the given rendering queue
	AKAME_API void RenderQueue(std::string QUEUENAME,Camera cam, unsigned int frameBuffer=0);
};

