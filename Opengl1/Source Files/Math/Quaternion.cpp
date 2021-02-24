#include "Quaternion.h"

Quaternion::Quaternion(glm::quat quat)
{
	quaternion = quat;

}

Quaternion::Quaternion(GLfloat w, GLfloat x, GLfloat y, GLfloat z)
{
	quaternion=glm::quat(w,x,y,z);

}

Quaternion::Quaternion(GLfloat EulerAngleX, GLfloat EulerAngleY, GLfloat EulerAngleZ)
{
	setEulerAngle(EulerAngleX,EulerAngleY,EulerAngleZ);
}

void Quaternion::setEulerAngle(GLfloat X, GLfloat Y, GLfloat Z)
{
	Quaternion qx=rotationAroundAxisVector(X,glm::vec3(1,0,0));
	Quaternion qy=rotationAroundAxisVector(Y,glm::vec3(0,1,0));
	Quaternion qz=rotationAroundAxisVector(Z,glm::vec3(0,0,1));
	*this = qx*qy*qz;
}

Quaternion Quaternion::rotationAroundAxisVector(GLfloat angleInDegrees, glm::vec3 AxisVector)
{
	AxisVector = glm::normalize(AxisVector);
	
	GLfloat rad = (angleInDegrees / 360.0) * glm::pi<GLfloat>() ;

	glm::quat quat(glm::cos(rad),glm::sin(rad)*AxisVector.x,glm::sin(rad)*AxisVector.y,glm::sin(rad)*AxisVector.z);
	return Quaternion(quat);
}
