#pragma once
#include"Components/Rendering/Mesh.h"

class AKAME_API DepthFrameBuffer
{

private:
	unsigned int framebuffer;
	unsigned int depthTex;
	int map_height, map_width;
	void generateFrameBuffer();

public:
	Mesh quadMesh;
	DepthFrameBuffer(int width = 1024, int  height = 1024);
	void updateTextureSize(int height, int width);


	void Bind();

	void unBind();

	void RenderToQuad();

	~DepthFrameBuffer();





};

