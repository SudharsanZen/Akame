#pragma once
#include"Rendering/System/LightSystem.h"
#include"Rendering/DeferredRendererFragmentBuffer.h"
#include"Assets/AssetManager.h"
class TiledRenderer
{
	unsigned int progID;
	
	unsigned int outTex;
	int height, width;
	
	char** readShaderFile(std::string fileName, int& len);
	void compileComputeShader(std::string fileName);
	void freeCodePointer(char** code, int len);
	void bindTextures();
	friend class RenderingSystem;
	
public:
	DeferredRendererFragmentBuffer drfb;
	TiledRenderer(std::string shaderLocation=AssetManager::assetRootPath+"Shaders/Deferred/Compute/defCal.comp");
	void set4x4Matrixfv(std::string, glm::mat4);
	void setFloat(std::string, float);
	void setInt(std::string, int);
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

