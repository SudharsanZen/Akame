#pragma once
#include"Rendering/Texture.h"
#include"Rendering/FrameBuffer.h"
#include"Assets/ShaderManager.h"
#include"Core\Scene.h"
#include"Core\Window.h"
#include"Core/Editor/AkameWindowBase.h"
class LightAndShadowConfig:public AkameWindowBase
{
private:
	Scene& m_scene;
	std::vector<FrameBuffer> fbo;
public:

	AKAME_API LightAndShadowConfig(Scene& scene);
	AKAME_API void Draw();
};
