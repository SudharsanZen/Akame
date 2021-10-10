#pragma once
#include"Components/Rendering/Mesh.h"
#include"Camera.h"
#include"Assets/ShaderManager.h"
namespace BasicShapes
{
	extern std::vector<vert> quadVert;
	extern std::vector<unsigned int> quadIndices;
}

class DeferredRendererFragmentBuffer
{
	

	//texture render target that are going to be attached to the frameBuffer
	unsigned int AlbedoRough=0, Normal=0, Position=0,depthBuffer=0,PBR=0;

	//render buffer for using as Depth/Stencil buffer
	unsigned int rbo=0;
	void GenerateFrameBuffer();
	unsigned int getAlbedoBuff() { return AlbedoRough; }
	unsigned int getDepthBuff() { return depthBuffer; }
	unsigned int getNormalBuff() { return this->Normal; }
	unsigned int getPositionBuff() { return Position; }
	friend class Editor;
	friend class TiledRenderer;
	friend class RenderingSystem;
public:
	Mesh quad;
	DeferredRendererFragmentBuffer();
	unsigned int frameBuffer = 0;
	
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

