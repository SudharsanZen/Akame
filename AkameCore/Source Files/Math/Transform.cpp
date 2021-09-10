#include "Components\Rendering\Transform.h"

//graphics framework uses left handsided 3d-coordinate system

Transform::Transform()
{
	baseScale = glm::vec3(1,1,1);
	basePosition = glm::vec3(0,0,0);
	localPosition = basePosition;
	localScale = baseScale;

	localToWorld =glm::mat4(1.0f);
	worldToLocal =glm::mat4(1.0f);
}

Transform::Transform(float posX, float posY, float posZ)
{
	baseScale = glm::vec3(1, 1, 1);
	basePosition = glm::vec3(0, 0, 0);
	localPosition = glm::vec3(posX,posY,posZ);
	localScale = baseScale;

	localToWorld = glm::mat4(1.0f);
	worldToLocal = glm::mat4(1.0f);
}
