#pragma once
#include"Systems/LightSystem.h"
#include"DeferredRendererFragmentBuffer.h"
#include"Assets/AssetManager.h"
class TiledRenderer
{
	unsigned int progID;
	
	unsigned int outTex;
	int height, width;
	DeferredRendererFragmentBuffer drfb;
	char** readShaderFile(std::string fileName, int& len);
	void compileComputeShader(std::string fileName);
	void freeCodePointer(char** code, int len);
	void bindTextures();

public:

	TiledRenderer(std::string shaderLocation=AssetManager::assetRootPath+"Shaders/Deferred/Compute/defCal.comp");

	void updateBufferSize(int height, int width);
	void bindFrameBuffer();
	void unBindFrameBuffer();
	
	void outPutToQaud(Camera& cam, std::shared_ptr<LightSystem> ls);

	void drawPositionBuffer();
	void drawNormalBuffer();
	void drawAlbedoBuffer();



};

