#pragma once
#include"Components/Rendering/Mesh.h"

class DepthFrameBuffer
{

private:
	unsigned int framebuffer;
	unsigned int depthTex;
	int map_height, map_width;
	AKAME_API void generateFrameBuffer();

public:
	Mesh quadMesh;
	AKAME_API DepthFrameBuffer(int width = 1024, int  height = 1024);
	AKAME_API void updateTextureSize(int height, int width);


	AKAME_API void Bind();

	AKAME_API void unBind();

	AKAME_API void RenderToQuad();

	AKAME_API ~DepthFrameBuffer();





};

