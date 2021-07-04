#include"Systems/LightSystem.h"
#include "Lights.h"
#include"EngineMathConstants.h"

Lights::Lights():Lights(LIGHT::POINT)
{
	

}

Lights::Lights(LIGHT ty)
{
	type = ty;
	lightDirection = worldDown;
	ambientLigting(0.2f,0.2f,0.2f);
	setColor(1,1,1);
	setIntensity(1);
	setPointLightConst(1,1,2);
}

void Lights::ambientLigting(glm::vec3 rgb) 
{
	ambient = rgb;
}

void Lights::ambientLigting(float r, float g,float b)
{
	ambient = glm::vec3(r,g,b);
}

LIGHT Lights::getType()
{
	return type;
}
void Lights::setColor(glm::vec3 rgb)
{
	lightColor = rgb;
}

void Lights::setColor(float r, float g, float b)
{
	setColor(glm::vec3(r,g,b));
}

void Lights::setDirection(float x, float y, float z)
{
	setDirection(glm::vec3(x,y,z));
}

void Lights::setDirection(glm::vec3 xyz)
{
	lightDirection = glm::normalize(xyz);
}

void Lights::setIntensity(float intensity)
{
	this->intensity = intensity;
}

void Lights::setPointLightConst(glm::vec3 ptc)
{
	this->pointLightConstants = ptc;
}

void Lights::setPointLightConst(float Kc, float Kl, float Kq)
{
	setPointLightConst(glm::vec3(Kc,Kl,Kq));
}
