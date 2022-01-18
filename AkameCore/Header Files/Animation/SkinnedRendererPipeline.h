#pragma once
#include"Components/Animation/AnimationController.h"
#include"Components/Animation/SkeletalMesh.h"
#include"Rendering/System/ShaderRenderPipeline.h"


class SkeletalMeshRenderingPipeline:public ShaderRenderPipeline
{
public:
	void OnPreRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam, unsigned int frameBuffer=0)override;
	void OnEntityRender(std::shared_ptr<Shader> skRend, std::shared_ptr<ECS> e, Camera cam, Entity eid)override;
};


