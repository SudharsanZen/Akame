#pragma once


namespace physics
{
	enum Shapes
	{
		BOX, PLANE, SPHERE
	};
	class ColliderShape
	{
		Shapes colShape;
		glm::vec3 dim;
		friend RigidBody3D;
	public:
		
		ColliderShape(Shapes sp=SPHERE,GLfloat d1=1,GLfloat d2=1,GLfloat d3=1)
		{
			setColliderShape(sp,d1,d2,d3);
		}

		/* for sphere d1 is the radius.
		for box d1,d2,d3 are the length, height, and width.
		for Plane d1 and d2 are the length and breadth.*/
		void setColliderShape(Shapes shape, GLfloat d1, GLfloat d2 = 1, GLfloat d3 = 1)
		{

			colShape = shape;
			dim = glm::vec3(d1,d2,d3);
		}

		Shapes getShapeType()
		{
			return colShape;
		}
	};
}