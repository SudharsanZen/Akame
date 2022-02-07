#pragma once
#include"Components/Rendering/Transform.h"
#include"Math/Quaternion.h"
#include<vector>
#include"Rendering/Camera.h"
#include"Components/Lighting/Lights.h"
std::vector<glm::mat4> CalculatePSSMLightSpaceMats(Camera &cam,glm::vec3 l,int numOfFrsutums,float lambda,float shadowDist);
std::vector<std::vector<glm::vec3>> CalculateFrustumCorners(Camera& cam, int numOfFrus,float lambda,float shadowDist);

class PSSMFBO
{
private:
	unsigned int fbo;
	unsigned int shadowMapsArray;
	int resolution;
	int numOfFrustum;
	AKAME_API void initialize();
	friend class Editor;
	friend class RenderingSystem;
	friend class LightSystem;
public:
	
	AKAME_API PSSMFBO(int numOfFrustum, int res);
	AKAME_API float GetResolution();
	AKAME_API void bind();
	AKAME_API void clearBuffer();

	AKAME_API void unbind(unsigned int frameBuffer=0);

	AKAME_API void useTextureArray(unsigned int unit);

};

