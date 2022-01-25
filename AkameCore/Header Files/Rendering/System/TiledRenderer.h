#pragma once
#include"Rendering/System/LightSystem.h"
#include"Rendering/DeferredRendererFragmentBuffer.h"
#include"Assets/AssetManager.h"
class TiledRenderer
{
	unsigned int progID;
	
	unsigned int outTex;
	int height, width;
	std::map<std::string, unsigned int> uniformNameToLoc;
	char** readShaderFile(std::string currPath, int& len);
	void compileComputeShader(std::string currPath);
	void freeCodePointer(char** code, int len);
	void bindTextures();
	friend class RenderingSystem;
	void ParseUniforms();
	unsigned int GetUniformLocation(std::string varName) { return uniformNameToLoc[varName]; }
public:
	DeferredRendererFragmentBuffer drfb;
	TiledRenderer(std::string shaderLocation=AssetManager::assetRootPath+"Shaders/Deferred/Compute/defCal.comp");
	void set4x4Matrixfv(std::string, glm::mat4);
	void set4x4MatrixfvArray(std::string name, unsigned int index, glm::mat4 value);
	void setVec3(std::string name, glm::vec3 value);
	void setFloat(std::string, float);
	void setInt(std::string, int);
	void updateBufferSize(int height, int width);
	void bindFrameBuffer();
	void unBindFrameBuffer(unsigned int frameBuffer=0);
	
	void setUpShader(Camera& cam, std::shared_ptr<LightSystem> ls);
	void outPutToQaud();

	void drawPositionBuffer();
	void drawNormalBuffer();
	void drawAlbedoBuffer();
	~TiledRenderer();



};

