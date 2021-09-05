#include "Math\Quaternion.h"
#include<glm\glm\mat3x3.hpp>
#include<glm\glm\glm.hpp>
#include<glad\glad.h>
glm::vec3 operator * ( glm::vec3 const& vec3, Quaternion& q)
{
/*glm::quat newVec = quaternion * vec3 * glm::conjugate(quaternion);
return glm::vec3(newVec.x, newVec.y, newVec.z);*/
glm::vec4 pos = q.getMatrix() * glm::vec4(vec3, 1);
return glm::vec3(pos.x, pos.y, pos.z);
}
glm::vec3 operator * (Quaternion& q,glm::vec3 const& vec3)
{
	/*glm::quat newVec = quaternion * vec3 * glm::conjugate(quaternion);
	return glm::vec3(newVec.x, newVec.y, newVec.z);*/
	glm::vec4 pos = q.getMatrix() * glm::vec4(vec3, 1);
	return glm::vec3(pos.x, pos.y, pos.z);
}
Quaternion operator *(const Quaternion& rhs, const Quaternion& lhs)
{
	Quaternion newQuat = (rhs.quaternion * lhs.quaternion);
	return newQuat;
}
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
	glm::quat qx = rotationAroundAxisVector(X, worldRight).quaternion;
	glm::quat qy = rotationAroundAxisVector(Y, worldUp).quaternion;
	glm::quat qz = rotationAroundAxisVector(Z, worldForward).quaternion;
	quaternion = qx * qy* qz;
}

Quaternion::operator std::string()const
{
	return std::string("quat: w:" + std::to_string(quaternion.w) + " x:" + std::to_string(quaternion.x) + " y:" + std::to_string(quaternion.y) + " z:" + std::to_string(quaternion.z));
}

Quaternion Quaternion::rotationAroundAxisVector(float angleInDegrees, glm::vec3 AxisVector)
{
	AxisVector = glm::normalize(AxisVector);

	float rad = glm::radians(angleInDegrees);

	/*glm::quat quat(glm::cos(rad), glm::sin(rad) * AxisVector.x, glm::sin(rad) * AxisVector.y, glm::sin(rad) * AxisVector.z);
	return Quaternion(quat);*/
	return glm::angleAxis(rad,AxisVector);
}