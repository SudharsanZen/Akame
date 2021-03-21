#pragma once
#include"EngineMathConstants.h"
#include"GlmMath.h"
#include"Quaternion.h"
class Transform
{
public:
	glm::vec3 position;
	glm::vec3 scale;
	Quaternion rotation;

	Transform();
	Transform(GLfloat posX,GLfloat posY,GLfloat posZ);
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