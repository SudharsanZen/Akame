#pragma once
#include<iostream>
#include<string>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"GlmMath.h"
#include<memory>
class Quaternion
{
private:
	

public:
	glm::quat quaternion;
	Quaternion(glm::quat quat);
	Quaternion(GLfloat w, GLfloat x, GLfloat y, GLfloat z);
	Quaternion(GLfloat EulerAngleX, GLfloat EulerAngleY, GLfloat EulerAngleZ);
	static Quaternion rotationAroundAxisVector(GLfloat angleInDegrees, glm::vec3 AxisVector);
	void setEulerAngle(GLfloat, GLfloat, GLfloat);
	Quaternion conjugate() { return glm::conjugate(quaternion); }




	operator std::string()const
	{

		return std::string("quat: w:" + std::to_string(quaternion.w) + " x:" + std::to_string(quaternion.x) + " y:" + std::to_string(quaternion.y) + " z:" + std::to_string(quaternion.z));
	}

	glm::mat4 getMatrix()
	{
		return glm::toMat4(quaternion);
	}
	Quaternion(Quaternion const& rValue)
	{
		quaternion = rValue.quaternion;
	}



	Quaternion& operator =(const Quaternion&& rhs)
	{
		this->quaternion = rhs.quaternion;
		return *this;
	}

	Quaternion operator *(const Quaternion& rhs)
	{
		return Quaternion(quaternion * rhs.quaternion);
	}

	glm::vec3 operator * (glm::vec3 const& vec3)
	{
		glm::quat newVec = quaternion * glm::quat(0, vec3) * glm::conjugate(quaternion);
		return glm::vec3(newVec.x, newVec.y, newVec.z);
	}




};
