#pragma once
#include"Components/Rendering/Mesh.h"
#include"Camera.h"
#include"Assets/ShaderManager.h"
namespace BasicShapes
{
	extern AKAME_API std::vector<vert> quadVert;
	extern AKAME_API std::vector<unsigned int> quadIndices;
}

class DeferredRendererFragmentBuffer
{
	

	//texture render target that are going to be attached to the frameBuffer
	unsigned int AlbedoRough=0, Normal=0, Position=0,depthBuffer=0,PBR=0;

	//render buffer for using as Depth/Stencil buffer
	unsigned int rbo=0;
	AKAME_API void GenerateFrameBuffer();
	AKAME_API unsigned int getAlbedoBuff();
	AKAME_API unsigned int getDepthBuff();
	AKAME_API unsigned int getNormalBuff();
	AKAME_API unsigned int getPositionBuff();
	friend class Editor;
	friend class TiledRenderer;
	friend class RenderingSystem;
public:
	Mesh quad;
	AKAME_API DeferredRendererFragmentBuffer();
	unsigned int frameBuffer = 0;
	
	AKAME_API void updateBufferSize(int height,int width);
	AKAME_API void bindFrameBuffer();
	AKAME_API void unBindFrameBuffer(unsigned int frameBuffer=0);
	//tex unit 1: albedoSpec,unit 2:Normal,unit 3:position
	AKAME_API void useTextures();
	AKAME_API void outPutToQaud(Camera& cam, std::shared_ptr<Shader> shader);

	AKAME_API void drawPositionBuffer();
	AKAME_API void drawNormalBuffer();
	AKAME_API void drawAlbedoBuffer();
	AKAME_API ~DeferredRendererFragmentBuffer();
};

