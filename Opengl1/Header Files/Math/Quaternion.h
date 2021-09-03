#pragma once
#include"Math/EngineMathConstants.h"
#include<string>
#include<memory>
class Quaternion
{
private:
	
	
public:
	glm::quat quaternion;
	
	Quaternion(glm::quat quat);
	Quaternion(float w, float x, float y, float z);
	Quaternion(float EulerAngleX, float EulerAngleY, float EulerAngleZ);
	static glm::quat rotationAroundAxisVector(float angleInDegrees, glm::vec3 AxisVector);
	void setEulerAngle(float, float, float);
	Quaternion conjugate() { return glm::conjugate(quaternion); }
	glm::vec3 getEulerAngle() { return glm::degrees(glm::eulerAngles(quaternion)); }




	operator std::string()const;

	glm::mat4 getMatrix()
	{
		return glm::toMat4(quaternion);
	}
	Quaternion(Quaternion const& rValue)
	{
		quaternion = rValue.quaternion;
	}



	Quaternion& operator =(const Quaternion&& rhs) noexcept
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
		/*glm::quat newVec = quaternion * vec3 * glm::conjugate(quaternion);
		return glm::vec3(newVec.x, newVec.y, newVec.z);*/
		glm::vec4 pos = getMatrix() * glm::vec4(vec3, 1);
		return glm::vec3(pos.x, pos.y, pos.z);
	}




};
