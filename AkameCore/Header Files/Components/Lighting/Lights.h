#pragma once
#include"glm/glm/glm.hpp"
#include"Rendering/Shader.h"
#include"Components/Components.h"
enum class LIGHT {DIRECTIONAL=0,POINT=1,SPOT=2};
class LightSystem;

class Lights:public Components
{
private:
	LIGHT type;
	glm::vec3 lightDirection;
	glm::vec3 lightColor;
	glm::vec3 ambient;

	glm::vec3 pointLightConstants;
	float intensity;

	friend class RenderingSystem;
	friend class InspectorWindow;
	friend class DeferredRendererFragmentBuffer;
	friend class LightSystem;
	friend class Editor;


	float pointLightRadius;
	
	void notifyLightSystemToUpdate();
public:
	float calPointLightRadius(glm::vec3 C, float intensity);
	float getPointLightRadius() { return calPointLightRadius(pointLightConstants,intensity); }
	Lights();
	Lights(LIGHT ty);
	
	void setColor(glm::vec3 rgb);
	void setColor(float r,float g,float b);
	
	void setDirection(float x,float y,float z);
	void setDirection(glm::vec3 angles);

	void ambientLigting(glm::vec3 rgb);
	void ambientLigting(float r,float g,float b);

	void setIntensity(float intenseity);
	void setPointLightConst(float Kc,float Kl,float Kq);
	void setPointLightConst(glm::vec3 constants);
	LIGHT getType();
	void setType(LIGHT ty) {
		type = ty;
		notifyLightSystemToUpdate();
	};
	void reset() {};
	glm::vec3 getDirection() { return glm::normalize(Quaternion(lightDirection.x,lightDirection.y,lightDirection.z)*worldForward); }
};

