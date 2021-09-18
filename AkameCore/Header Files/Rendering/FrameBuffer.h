#pragma once
#include"Components/Rendering/Mesh.h"

class FrameBuffer
{
private:
	unsigned int framebuffer;
	unsigned int rbo;
	unsigned int texColorBuffer;
	int height, width;
	void generateFrameBuffer();
	
public:
	Mesh quadMesh;
	FrameBuffer();
	void updateTextureSize(int height,int width);


	void Bind();

	void unBind();

	void RenderToQuad();

	unsigned int getColorBuffer() { return texColorBuffer; }
	~FrameBuffer();


};

