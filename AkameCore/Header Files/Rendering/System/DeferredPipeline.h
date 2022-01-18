#pragma once
#include<glad\glad.h>
#include "Rendering\System\ShaderRenderPipeline.h"
#include "Rendering\System\RenderingSystem.h"
#include "Rendering\System\LightSystem.h"
#include"Rendering\System\TiledRenderer.h"
#include"Math\GlmMath.h"

class DeferredPipeline :public ShaderRenderPipeline
{
	TiledRenderer drfb;
	friend class Editor;
public:
	DeferredPipeline(int height,int width) { drfb.updateBufferSize(height, width); }
	void  WindowsResizeCallBacks(int height, int width) override;
    void OnPreRender(std::shared_ptr<Shader> shader,RenderingSystem* rsys, Camera cam, unsigned int frameBuffer=0) override;
	void OnPostRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam, unsigned int frameBuffer=0) override;
};

class RM_SKY_BOXPipeline :public ShaderRenderPipeline
{
public:
	
	void OnPreRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam, unsigned int frameBuffer=0) override;
	void OnPostRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam, unsigned int frameBuffer=0) override;
};