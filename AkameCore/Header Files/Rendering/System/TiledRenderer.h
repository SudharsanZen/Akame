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
	AKAME_API char** readShaderFile(std::string currPath, int& len);
	AKAME_API void compileComputeShader(std::string currPath);
	AKAME_API void freeCodePointer(char** code, int len);
	AKAME_API void bindTextures();
	AKAME_API friend class RenderingSystem;
	AKAME_API void ParseUniforms();
	AKAME_API unsigned int GetUniformLocation(std::string varName);
public:
	DeferredRendererFragmentBuffer drfb;
	AKAME_API TiledRenderer(std::string shaderLocation=AssetManager::assetRootPath+"Shaders/Deferred/Compute/defCal.comp");
	AKAME_API void set4x4Matrixfv(std::string, glm::mat4);
	AKAME_API void set4x4MatrixfvArray(std::string name, unsigned int index, glm::mat4 value);
	AKAME_API void setVec3(std::string name, glm::vec3 value);
	AKAME_API void setFloat(std::string, float);
	AKAME_API void setInt(std::string, int);
	AKAME_API void updateBufferSize(int height, int width);
	AKAME_API void bindFrameBuffer();
	AKAME_API void unBindFrameBuffer(unsigned int frameBuffer=0);
	
	AKAME_API void setUpShader(Camera& cam, std::shared_ptr<LightSystem> ls);
	AKAME_API void outPutToQaud();

	AKAME_API void drawPositionBuffer();
	AKAME_API void drawNormalBuffer();
	AKAME_API void drawAlbedoBuffer();
	AKAME_API ~TiledRenderer();



};

