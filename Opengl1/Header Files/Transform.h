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
		return rotation*worldForward;
	}
	glm::vec3 backward()
	{
		return -forward();
	}
	glm::vec3 right()
	{
		return rotation*worldRight;
	}
	glm::vec3 left()
	{
		return -right();
	}
	glm::vec3 up()
	{
		return rotation * worldUp;
	}
	glm::vec3 down()
	{
		return -up();
	}
};