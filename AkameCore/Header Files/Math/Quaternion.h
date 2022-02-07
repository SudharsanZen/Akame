#pragma once
#include"Math/EngineMathConstants.h"
#include<string>
#include<memory>
#include"Core/AkameCore.h"
class Quaternion;
AKAME_API glm::vec3 operator * (glm::vec3 const& vec3, Quaternion& q);
AKAME_API glm::vec3 operator * ( Quaternion& q, glm::vec3 const& vec3);
AKAME_API Quaternion operator *(const Quaternion& rhs, const Quaternion& lhs);
class Quaternion
{
private:
	
	
public:
	glm::quat quaternion;
	AKAME_API Quaternion();
	AKAME_API Quaternion(glm::quat quat);
	AKAME_API Quaternion(float w, float x, float y, float z);
	AKAME_API Quaternion(float EulerAngleX, float EulerAngleY, float EulerAngleZ);
	AKAME_API static Quaternion rotationAroundAxisVector(float angleInDegrees, glm::vec3 AxisVector);
	AKAME_API void setEulerAngle(float, float, float);
	AKAME_API Quaternion conjugate();
	AKAME_API Quaternion inverse();
	AKAME_API glm::vec3 getEulerAngle();

	AKAME_API operator std::string()const;

	AKAME_API glm::mat4 getMatrix();
	AKAME_API Quaternion(Quaternion const& rValue);



	Quaternion& operator =(const Quaternion&& rhs) noexcept
	{
		this->quaternion = rhs.quaternion;
		return *this;
	}

	friend AKAME_API Quaternion operator *(const Quaternion& rhs, const Quaternion& lhs);
	

	friend AKAME_API glm::vec3 operator * ( glm::vec3 const& vec3, Quaternion& q);
	friend AKAME_API glm::vec3 operator * (Quaternion& q, glm::vec3 const& vec3);

	Quaternion& operator = (Quaternion const& quat)
	{
		quaternion = quat.quaternion;
		return *this;
	}




};
