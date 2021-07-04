#pragma once
#include"glm/glm/glm.hpp"
#include"Shader.h"

enum class LIGHT {DIRECTIONAL,POINT,SPOT};
class LightSystem;
class Lights
{
private:
	LIGHT type=LIGHT::POINT;
	glm::vec3 lightDirection;
	glm::vec3 lightColor;
	glm::vec3 ambient;

	glm::vec3 pointLightConstants;
	float intensity;

	friend class RenderingSystem;
	friend class DeferredRendererFragmentBuffer;
	friend class LightSystem;


public:
	Lights();
	Lights(LIGHT ty);

	void setColor(glm::vec3 rgb);
	void setColor(float r,float g,float b);
	
	void setDirection(float x,float y,float z);
	void setDirection(glm::vec3 direction);

	void ambientLigting(glm::vec3 rgb);
	void ambientLigting(float r,float g,float b);

	void setIntensity(float intenseity);
	void setPointLightConst(float Kc,float Kl,float Kq);
	void setPointLightConst(glm::vec3 constants);
	LIGHT getType();

	void reset() {};
};

