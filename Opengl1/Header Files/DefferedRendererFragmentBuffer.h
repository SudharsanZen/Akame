#pragma once
#include"Mesh.h"
#include"Camera.h"
class DeferredRendererFragmentBuffer
{
	Mesh quad;
	unsigned int frameBuffer=0;
	//texture render target that are going to be attached to the frameBuffer
	unsigned int AlbedoSpec=0, Normal=0, Position=0;

	//render buffer for using as Depth/Stencil buffer
	unsigned int rbo=0;
	void GenerateFrameBuffer();
public:

	DeferredRendererFragmentBuffer();

	
	void updateBufferSize(int height,int width);
	void bindFrameBuffer();
	void unBindFrameBuffer();
	//tex unit 1: albedoSpec,unit 2:Normal,unit 3:position
	void useTextures();
	void outPutToQaud(Camera &cam);
	~DeferredRendererFragmentBuffer();
};

