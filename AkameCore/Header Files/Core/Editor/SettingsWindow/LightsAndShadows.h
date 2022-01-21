#pragma once
#include"Rendering/Texture.h"
#include"Rendering/FrameBuffer.h"
#include"Assets/ShaderManager.h"
#include"Core\Scene.h"
#include"Core\Window.h"

class LightAndShadowConfig
{
private:
	Scene& m_scene;
	std::vector<FrameBuffer> fbo;
public:

	LightAndShadowConfig(Scene& scene);
	void Draw();
};
