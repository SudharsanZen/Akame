#include "Quaternion.h"
#include<glm\glm\mat3x3.hpp>
#include<glm\glm\glm.hpp>
#include<glad\glad.h>
Quaternion::Quaternion(glm::quat quat)
{
	quaternion = quat;

}

Quaternion::Quaternion(float w, float x, float y, float z)
{
	quaternion = glm::quat(w, x, y, z);

}

Quaternion::Quaternion(float EulerAngleX, float EulerAngleY, float EulerAngleZ)
{
	setEulerAngle(EulerAngleX, EulerAngleY, EulerAngleZ);
}

void Quaternion::setEulerAngle(float X, float Y, float Z)
{
	glm::quat qx = rotationAroundAxisVector(X, worldRight);
	glm::quat qy = rotationAroundAxisVector(Y, worldUp);
	glm::quat qz = rotationAroundAxisVector(Z, worldForward);
	quaternion = qx * qy* qz;
}

Quaternion::operator std::string()const
{
	return std::string("quat: w:" + std::to_string(quaternion.w) + " x:" + std::to_string(quaternion.x) + " y:" + std::to_string(quaternion.y) + " z:" + std::to_string(quaternion.z));
}

glm::quat Quaternion::rotationAroundAxisVector(float angleInDegrees, glm::vec3 AxisVector)
{
	AxisVector = glm::normalize(AxisVector);

	float rad = glm::radians(angleInDegrees);

	/*glm::quat quat(glm::cos(rad), glm::sin(rad) * AxisVector.x, glm::sin(rad) * AxisVector.y, glm::sin(rad) * AxisVector.z);
	return Quaternion(quat);*/
	return glm::angleAxis(rad,AxisVector);
}