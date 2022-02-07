#pragma once
#include"Rendering/System/ShaderRenderPipeline.h"

class DefaultRenderingPipeline:public ShaderRenderPipeline
{
private:
	AKAME_API void  WindowsResizeCallBacks(int height, int width) override;
	AKAME_API void OnPreRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam, unsigned int frameBuffer=0) override;
	AKAME_API void OnPostRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam, unsigned int frameBuffer=0) override;
};

