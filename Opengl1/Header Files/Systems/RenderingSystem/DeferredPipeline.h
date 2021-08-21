#pragma once
#include<glad/glad.h>
#include "Systems\RenderingSystem\ShaderRenderPipeline.h"
#include "Systems\RenderingSystem\RenderingSystem.h"
#include "Systems/RenderingSystem/LightSystem.h"
#include"Systems/RenderingSystem/TiledRenderer.h"
#include"GlmMath.h"

class DeferredPipeline :public ShaderRenderPipeline
{
	TiledRenderer drfb;
	
public:
	void  WindowsResizeCallBacks(int height, int width) override;
    void OnPreRender(std::shared_ptr<Shader> shader,RenderingSystem* rsys, Camera cam) override;
	void OnPostRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam) override;
};

class RM_SKY_BOXPipeline :public ShaderRenderPipeline
{
public:
	
	void OnPreRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam) override;
	void OnPostRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam) override;
};