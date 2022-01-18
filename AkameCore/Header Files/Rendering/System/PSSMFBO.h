#pragma once
#include"Components/Rendering/Transform.h"
#include"Math/Quaternion.h"
#include<vector>
#include"Rendering/Camera.h"
#include"Components/Lighting/Lights.h"
std::vector<glm::mat4> CalculatePSSMLightSpaceMats(Camera &cam,glm::vec3 l,int numOfFrsutums,float lambda,float shadowDist);
std::vector<std::vector<glm::vec3>> CalculateFrustumCorners(Camera& cam, int numOfFrus,float lambda,float shadowDist);
extern float pssmx;
extern float pssmy;
class PSSMFBO
{
private:
	unsigned int fbo;
	unsigned int shadowMapsArray;
	int resolution;
	int numOfFrustum;
	void initialize();
	friend class Editor;
	friend class RenderingSystem;
	friend class LightSystem;
public:
	
	PSSMFBO(int numOfFrustum, int res);
	float GetResolution() { return resolution; }
	void bind();
	void clearBuffer();

	void unbind(unsigned int frameBuffer=0);

	void useTextureArray(unsigned int unit);

};

