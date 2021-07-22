#pragma once
#include"Systems/RenderingSystem/LightSystem.h"
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
	void set4x4Matrixfv(std::string, glm::mat4);
	void updateBufferSize(int height, int width);
	void bindFrameBuffer();
	void unBindFrameBuffer();
	
	void setUpShader(Camera& cam, std::shared_ptr<LightSystem> ls);
	void outPutToQaud();

	void drawPositionBuffer();
	void drawNormalBuffer();
	void drawAlbedoBuffer();
	~TiledRenderer();



};

