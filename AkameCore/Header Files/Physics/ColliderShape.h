#pragma once
#include"Math/GlmMath.h"

namespace physics
{
	enum class Shapes
	{
		BOX, PLANE, SPHERE
	};
	class ColliderShape
	{
		Shapes colShape;
		glm::vec3 dim;
		friend class RigidBody3D;

	public:
		
		ColliderShape(Shapes sp=Shapes::SPHERE, float d1 = 1, float d2 = 1, float d3 = 1)
		{
			setColliderShape(sp,d1,d2,d3);
		}

		/* for sphere d1 is the radius.
		for box d1,d2,d3 are the length, height, and width.
		for Plane d1 and d2 are the length and breadth.*/
		void setColliderShape(Shapes shape, float d1, float d2 = 1, float d3 = 1)
		{

			colShape = shape;
			dim = glm::vec3(d1,d2,d3);
		}
		glm::vec3 getColliderDimensions()
		{
			return dim;
		}
		Shapes getShapeType()
		{
			return colShape;
		}
	};
}