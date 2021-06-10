#include "Transform.h"

//graphics framework uses left handsided 3d-coordinate system

Transform::Transform():rotation(0,0,0)
{
	scale = glm::vec3(1,1,1);
	position = glm::vec3(0,0,0);
}

Transform::Transform(float posX, float posY, float posZ):rotation(0,0,0)
{
	scale = glm::vec3(1,1,1);
	position = glm::vec3(posX,posY,posZ);
}
