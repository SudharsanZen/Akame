#pragma once
#include"Components/Rendering/Mesh.h"

class FrameBuffer
{
private:
	unsigned int framebuffer;
	unsigned int rbo;
	unsigned int texColorBuffer;
	int height, width;
	AKAME_API void generateFrameBuffer();
	friend class ViewPortWindow;
public:
	Mesh quadMesh;
	AKAME_API FrameBuffer();
	AKAME_API void updateTextureSize(int height,int width);
	AKAME_API glm::vec2 getFrameSize();

	AKAME_API void Bind();

	AKAME_API void unBind();

	AKAME_API void RenderToQuad();

	AKAME_API unsigned int getColorBuffer();
	AKAME_API ~FrameBuffer();


};

