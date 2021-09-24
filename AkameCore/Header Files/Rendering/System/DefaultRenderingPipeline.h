#pragma once
#include"Rendering/System/ShaderRenderPipeline.h"
class DefaultRenderingPipeline:public ShaderRenderPipeline
{
private:
	void  WindowsResizeCallBacks(int height, int width) override;
	void OnPreRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam) override;
	void OnPostRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam) override;
};

