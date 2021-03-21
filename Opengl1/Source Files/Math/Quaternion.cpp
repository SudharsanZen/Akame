#include "Quaternion.h"
#include<glm\glm\mat3x3.hpp>

Quaternion::Quaternion(glm::quat quat)
{
	quaternion = quat;

}

Quaternion::Quaternion(GLfloat w, GLfloat x, GLfloat y, GLfloat z)
{
	quaternion = glm::quat(w, x, y, z);

}

Quaternion::Quaternion(GLfloat EulerAngleX, GLfloat EulerAngleY, GLfloat EulerAngleZ)
{
	setEulerAngle(EulerAngleX, EulerAngleY, EulerAngleZ);
}

void Quaternion::setEulerAngle(GLfloat X, GLfloat Y, GLfloat Z)
{
	glm::quat qx = rotationAroundAxisVector(X, worldRight);
	glm::quat qy = rotationAroundAxisVector(Y, worldUp);
	glm::quat qz = rotationAroundAxisVector(Z, worldForward);
	quaternion = qx * qy;
}

glm::quat Quaternion::rotationAroundAxisVector(GLfloat angleInDegrees, glm::vec3 AxisVector)
{
	AxisVector = glm::normalize(AxisVector);

	GLfloat rad = (angleInDegrees / 360.0) * glm::pi<GLfloat>();

	/*glm::quat quat(glm::cos(rad), glm::sin(rad) * AxisVector.x, glm::sin(rad) * AxisVector.y, glm::sin(rad) * AxisVector.z);
	return Quaternion(quat);*/
	return glm::angleAxis(rad,AxisVector);
}