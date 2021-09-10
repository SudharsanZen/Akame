#pragma once
#include"Math/EngineMathConstants.h"
#include<string>
#include<memory>
class Quaternion;
glm::vec3 operator * (glm::vec3 const& vec3, Quaternion& q);
glm::vec3 operator * ( Quaternion& q, glm::vec3 const& vec3);
Quaternion operator *(const Quaternion& rhs, const Quaternion& lhs);
class Quaternion
{
private:
	
	
public:
	glm::quat quaternion;
	Quaternion() :Quaternion(glm::quat(1,0,0,0)) {};
	Quaternion(glm::quat quat);
	Quaternion(float w, float x, float y, float z);
	Quaternion(float EulerAngleX, float EulerAngleY, float EulerAngleZ);
	static Quaternion rotationAroundAxisVector(float angleInDegrees, glm::vec3 AxisVector);
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

	friend Quaternion operator *(const Quaternion& rhs, const Quaternion& lhs);
	

	friend glm::vec3 operator * ( glm::vec3 const& vec3, Quaternion& q);
	friend glm::vec3 operator * (Quaternion& q, glm::vec3 const& vec3);

	Quaternion& operator = (Quaternion const& quat)
	{
		quaternion = quat.quaternion;
		return *this;
	}




};
