#pragma once
#include"Mesh.h"
#include"Camera.h"
#include"Assets/ShaderManager.h"
namespace BasicShapes
{
	extern std::vector<vert> quadVert;
	extern std::vector<unsigned int> quadIndices;
}

class DeferredRendererFragmentBuffer
{
	Mesh quad;
	unsigned int frameBuffer=0;
	//texture render target that are going to be attached to the frameBuffer
	unsigned int AlbedoSpec=0, Normal=0, Position=0,depthBuffer=0;

	//render buffer for using as Depth/Stencil buffer
	unsigned int rbo=0;
	void GenerateFrameBuffer();

	friend class TiledRenderer;
	friend class RenderingSystem;
public:

	DeferredRendererFragmentBuffer();

	
	void updateBufferSize(int height,int width);
	void bindFrameBuffer();
	void unBindFrameBuffer();
	//tex unit 1: albedoSpec,unit 2:Normal,unit 3:position
	void useTextures();
	void outPutToQaud(Camera& cam, std::shared_ptr<Shader> shader);

	void drawPositionBuffer();
	void drawNormalBuffer();
	void drawAlbedoBuffer();
	~DeferredRendererFragmentBuffer();
};

