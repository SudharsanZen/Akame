#pragma once
#include"Core/Debug/Debug.h"
#include"Math/GlmMath.h"
#include<memory>
#include<set>
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
class SkeletalMeshRenderingSystem: public System
{
	friend class Scene;
	std::weak_ptr<ECS> ecs;

	bool needs_update;

	std::map<std::string, std::vector<Entity>> mShaderNameEntityMap;
public:
	AKAME_API SkeletalMeshRenderingSystem();
	AKAME_API void OnAddEntity(Entity eid)override;
	AKAME_API void OnDestroyEntity(Entity eid)override;
	AKAME_API void renderMeshes(RenderingSystem* rendSys, Camera cam);

	AKAME_API void RenderSkeletalMeshQueue(RenderingSystem* rendSys,Camera cam);
	AKAME_API void RenderEntitiesWithShader(RenderingSystem* rendSys,std::string shaderName, Camera cam);

	AKAME_API void RenderShadows(RenderingSystem* rendSys, std::shared_ptr<Shader> skrend, Camera cam);

	AKAME_API void  Run();
};

