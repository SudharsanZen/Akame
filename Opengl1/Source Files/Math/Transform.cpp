#include "Transform.h"

Transform::Transform():rotation(0,0,0)
{
	scale = glm::vec3(1,1,1);
	position = glm::vec3(0,0,0);
}

Transform::Transform(GLfloat posX, GLfloat posY, GLfloat posZ):rotation(0,0,0)
{
	scale = glm::vec3(1,1,1);
	position = glm::vec3(posX,posY,posZ);
}
