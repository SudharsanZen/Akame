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
	AKAME_API DeferredPipeline(int height, int width);
	AKAME_API void  WindowsResizeCallBacks(int height, int width) override;
	AKAME_API void OnPreRender(std::shared_ptr<Shader> shader,RenderingSystem* rsys, Camera cam, unsigned int frameBuffer=0) override;
	AKAME_API void OnPostRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam, unsigned int frameBuffer=0) override;
};

class RM_SKY_BOXPipeline :public ShaderRenderPipeline
{
public:
	
	AKAME_API void OnPreRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam, unsigned int frameBuffer=0) override;
	AKAME_API void OnPostRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam, unsigned int frameBuffer=0) override;
};