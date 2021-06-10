#pragma once
#include"EngineMathConstants.h"
#include"Quaternion.h"
class Transform
{
public:
	glm::vec3 position;
	glm::vec3 scale;
	Quaternion rotation;
	void reset()
	{
		position = glm::vec3(0);
		scale = glm::vec3(1);
		rotation.setEulerAngle(0,0,0);
	}
	Transform();
	Transform(float posX,float posY,float posZ);
	Transform(glm::vec3 vec):Transform(vec.x,vec.y,vec.z) {};
	Transform& operator =(const Transform& Tran)
	{
		position = Tran.position;
		scale = Tran.scale;
		rotation.quaternion = Tran.rotation.quaternion;
		return *this;
	}
	glm::mat4 transformMatrix()
	{
		glm::mat4 trans = glm::mat4(1.0f);
		trans=glm::translate(trans,position);
		trans = trans * rotation.getMatrix();
		trans = glm::scale(trans, scale);
		return trans;
	}
	glm::vec3 forward()
	{
		return glm::normalize(rotation*worldForward);
	}
	glm::vec3 backward()
	{
		return -forward();
	}
	glm::vec3 right()
	{
		return -left();
	}
	glm::vec3 left()
	{
		return glm::normalize(rotation * worldLeft);
	}
	glm::vec3 up()
	{
		return glm::normalize(rotation * worldUp);
	}
	glm::vec3 down()
	{
		return -up();
	}
};