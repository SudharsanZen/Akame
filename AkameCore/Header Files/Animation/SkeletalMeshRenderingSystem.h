#pragma once
#include"Core/Debug/Debug.h"
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
#include"Components/Animation/AnimationController.h"
#include"Components/Rendering/Material.h"
#include<sstream>
#include"Assets/ShaderManager.h"
class AKAME_API SkeletalMeshRenderingSystem: public System
{
	friend class Scene;
	std::weak_ptr<ECS> ecs;

	bool needs_update;

	std::map<std::string, std::vector<Entity>> mShaderNameEntityMap;
public:
	SkeletalMeshRenderingSystem()
	{
		needs_update = true;
	}
	void OnAddEntity(Entity eid)override
	{
		needs_update = true;
	}
	void OnDestroyEntity(Entity eid)override
	{
		needs_update = true;
	}
	void renderMeshes(RenderingSystem* rendSys,Camera cam)
	{
		RenderSkeletalMeshQueue(rendSys,cam);
	}

	void RenderSkeletalMeshQueue(RenderingSystem* rendSys,Camera cam);
	void RenderEntitiesWithShader(RenderingSystem* rendSys,std::string shaderName, Camera cam);

	void RenderShadows(RenderingSystem* rendSys, std::shared_ptr<Shader> skrend, Camera cam);

	void Run();
};

