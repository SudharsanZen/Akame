#pragma once
#include"Components/Rendering/Mesh.h"

class AKAME_API FrameBuffer
{
private:
	unsigned int framebuffer;
	unsigned int rbo;
	unsigned int texColorBuffer;
	int height, width;
	void generateFrameBuffer();
	friend class ViewPortWindow;
public:
	Mesh quadMesh;
	FrameBuffer();
	void updateTextureSize(int height,int width);
	glm::vec2 getFrameSize() { return glm::vec2(width, height); }

	void Bind();

	void unBind();

	void RenderToQuad();

	unsigned int getColorBuffer() { return texColorBuffer; }
	~FrameBuffer();


};

