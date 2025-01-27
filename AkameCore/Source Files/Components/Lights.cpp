#include"Rendering/System/LightSystem.h"
#include "Components/Lighting/Lights.h"
#include"Math/EngineMathConstants.h"

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
	notifyLightSystemToUpdate();
}

void Lights::ambientLigting(float r, float g,float b)
{
	ambient = glm::vec3(r,g,b);
	notifyLightSystemToUpdate();
}

LIGHT Lights::getType()
{
	return type;
}
void Lights::setType(LIGHT ty) {
	type = ty;
	notifyLightSystemToUpdate();
}
void Lights::reset() 
{

}
AKAME_API glm::vec3 Lights::getDirection() 
{ 
	return glm::normalize(Quaternion(lightDirection.x, lightDirection.y, lightDirection.z) * worldForward); 
}
void Lights::setColor(glm::vec3 rgb)
{
	notifyLightSystemToUpdate();
	lightColor = rgb;
	
}

void Lights::setColor(float r, float g, float b)
{
	notifyLightSystemToUpdate();
	setColor(glm::vec3(r,g,b));
}

void Lights::setDirection(float x, float y, float z)
{
	notifyLightSystemToUpdate();
	setDirection(glm::vec3(x,y,z));
}

void Lights::setDirection(glm::vec3 xyz)
{
	notifyLightSystemToUpdate();
	lightDirection = xyz;
}

void Lights::setIntensity(float intensity)
{
	notifyLightSystemToUpdate();
	this->intensity = intensity;
	pointLightRadius = calPointLightRadius(pointLightConstants,intensity);

}

void Lights::setPointLightConst(glm::vec3 ptc)
{
	notifyLightSystemToUpdate();
	this->pointLightConstants = ptc;
	pointLightRadius = calPointLightRadius(pointLightConstants, intensity);
	//ENGINE_CORE_TRACE("point light radius: {0:f}",pointLightRadius);
}

void Lights::setPointLightConst(float Kc, float Kl, float Kq)
{
	notifyLightSystemToUpdate();
	setPointLightConst(glm::vec3(Kc,Kl,Kq));
}

void Lights::notifyLightSystemToUpdate()
{

	LightSystem::needsUpdate = true;
}

float Lights::calPointLightRadius(glm::vec3 C,float Intensity)
{
	float kc=C.x, kl=C.y, kq=C.z;
	float r1,r2;
	float goalAtten = 0.02f;
	float D = sqrt((kl*kl)-4.0f*kq*(kc-(intensity/goalAtten)));
	
	r1 = -kl - D;
	r2 = -kl + D;

	r1 /= 2.0f * kq;
	r2 /= 2.0f * kq;

	return glm::max(r1,r2);

}

float Lights::getPointLightRadius() { return calPointLightRadius(pointLightConstants, intensity); }
