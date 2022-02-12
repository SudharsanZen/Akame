#include "Components\Rendering\Transform.h"

//graphics framework uses left handsided 3d-coordinate system


/*
AK_SERIALIZABLES
(
AK_ID(localPosition)
AK_ID(localScale)
AK_ID(localRotation)
AK_ID_COMPX_LIST(child)
AK_ID_COMPX(parent)
)*/

glm::mat4 Transform::formTransformMatrix(glm::vec3 position, Quaternion rotation, glm::vec3 scale)
{

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, position);
	trans = trans * rotation.getMatrix();
	trans = glm::scale(trans, scale);
	return trans;
}

void Transform::updateChildBaseTransformDetails()
{
	std::shared_ptr<ECS> E = ecs.lock();

	transformMat = transformMatrix();
	if (ecs.expired())
	{
		std::cout << "expired";
	}
	e_index transformIndex = E->GetComponentBitPose<Transform>();
	for (auto e : child)
	{
		Transform& t = E->GetComponent<Transform>((*e.componentIndex)[transformIndex], transformIndex);
		t.basePosition = GetGlobalPosition();
		t.baseRotation = GetGlobalRotation();
		t.baseScale = GetGlobalScale();

		//get conversion matrices 

		t.localToWorld = transformMatrix();
		t.worldToLocal = glm::inverse(t.localToWorld);
		t.updateChildBaseTransformDetails();
	}
}

void Transform::_addToUpdateTransformList()
{
	if (transformUpdateList)
		transformUpdateList->insert(componentIndex);
}

void Transform::destroyChildren()
{
	/*since removeParent (called on destroy of entities with transform components) changes the original child list, to avoid access violation error
	we are using a copy of the initial child list before destroy	and then doing the loop
	*/
	auto eCopy = child;
	for (auto e : eCopy)
	{
		Transform& t = ecs.lock()->GetComponent<Transform>(e);
		t.destroyChildren();
		ecs.lock()->DestroyEntity(e);

	}
}

glm::mat4 Transform::getCalculatedTransform()
{
	return transformMat;
}

Transform::Transform()
{
	eid = Entity(-1,-1);

	baseScale = glm::vec3(1,1,1);
	localScale = baseScale;

	basePosition = glm::vec3(0,0,0);
	localPosition = basePosition;
	
	localRotation = glm::quat(1,0,0,0);
	baseRotation= glm::quat(1,0,0,0);

	localToWorld =glm::mat4(1.0f);
	worldToLocal =glm::mat4(1.0f);
}

Transform::Transform(float posX, float posY, float posZ):Transform()
{
	localPosition = glm::vec3(posX, posY, posZ);
}


Transform::Transform(glm::vec3 vec) :Transform(vec.x, vec.y, vec.z) 
{

}

glm::vec3 Transform::GetLocalPosition()
{
	return localPosition;
}

glm::vec3 Transform::GetLocalScale()
{
	return localScale;
}

Quaternion Transform::GetLocalRotation()
{
	return localRotation;
}

glm::vec3 Transform::GetGlobalPosition()
{

	return transformMatrix() * glm::vec4(0, 0, 0, 1);
}

Quaternion Transform::GetGlobalRotation()
{
	return baseRotation * localRotation;
}

glm::vec3 Transform::GetGlobalScale()
{
	return baseScale * localScale;
}

void Transform::SetLocalPosition(glm::vec3 position)
{
	_addToUpdateTransformList();
	localPosition = position;
}

void Transform::SetLocalScale(glm::vec3 scale)
{
	_addToUpdateTransformList();
	localScale = scale;
}

void Transform::SetGlobalPosition(glm::vec3 position)
{
	_addToUpdateTransformList();
	localPosition = worldToLocal * glm::vec4(position, 1);
}

void Transform::SetGlobalRotation(Quaternion rotation)
{
	_addToUpdateTransformList();
	if (parent == Entity(-1, -1))
		localRotation = rotation.quaternion;
	else
		localRotation = glm::inverse(baseRotation.quaternion) * rotation.quaternion;
}

void Transform::SetGlobalScale(glm::vec3 scale)
{
	_addToUpdateTransformList();
	localScale = scale / baseScale;
}

std::list<Entity>& Transform::getChildList()
{
	return child;
}

Transform& Transform::getParentTransform()
{
	assert(parent != Entity(-1, -1) && "trying to access non existing parent transform, this transform doesn't have any parent");
	return ecs.lock()->GetComponent<Transform>(parent);
}

Entity Transform::getParentId()
{
	assert(parent != Entity(-1, -1) && "trying to access non existing parent transform, this transform doesn't have any parent");
	return parent;
}

void Transform::setParent(Entity parentEID)
{
	//get world transform details of this transform 
	glm::vec3 globalPose = GetGlobalPosition();
	glm::vec3 globalScale = GetGlobalScale();
	Quaternion globalRotation = GetGlobalRotation();

	removeParent();

	//insert itself into the child list of the new parent
	parent = parentEID;
	Transform& p = ecs.lock()->GetComponent<Transform>(parent);
	p.child.push_back(eid);
	//get parent transform details
	baseRotation = p.GetGlobalRotation();
	basePosition = p.GetGlobalPosition();
	baseScale = p.GetGlobalScale();

	//get conversion matrices 
	localToWorld = p.transformMatrix();
	worldToLocal = glm::inverse(localToWorld);

	//convert previously calculated world transform details to the current parent's local space
	localPosition = worldToLocal * glm::vec4(globalPose, 1);
	localRotation = glm::inverse(baseRotation.quaternion) * globalRotation.quaternion;
	localScale = globalScale / baseScale;

	updateChildBaseTransformDetails();


}

void Transform::removeParent()
{
	//remove the current entity ID from it's current parent's child list
	localPosition = GetGlobalPosition();
	localScale = GetGlobalScale();
	localRotation = GetGlobalRotation();

	if (parent != Entity(-1, -1))
	{
		Transform& prevP = ecs.lock()->GetComponent<Transform>(parent);
		auto itr = std::find(prevP.child.begin(), prevP.child.end(), eid);

		if (itr != prevP.child.end())
		{
			prevP.child.erase(itr);
		}

	}
	parent = Entity(-1, -1);
	basePosition = glm::vec3(0);
	baseScale = glm::vec3(1);
	baseRotation = Quaternion(1, 0, 0, 0);
	localToWorld = glm::mat4(1.0f);
	worldToLocal = glm::mat4(1.0f);
	updateChildBaseTransformDetails();

}

void Transform::reset()
{
	auto cChild = child;
	for (auto ch : cChild)
	{
		Transform& t = ecs.lock()->GetComponent<Transform>(ch);
		t.setParent(parent);
	}
	removeParent();

	basePosition = glm::vec3(0);
	baseScale = glm::vec3(1);
	baseRotation.setEulerAngle(0, 0, 0);
	localPosition = glm::vec3(0);
	localScale = glm::vec3(1);
	localRotation.setEulerAngle(0, 0, 0);
	localToWorld = glm::mat4(1.0f);
	worldToLocal = glm::mat4(1.0f);
}

glm::mat4 Transform::transformMatrix()
{

	glm::mat4 trans = glm::translate(glm::mat4(1.0f), localPosition) * localRotation.getMatrix() * glm::scale(glm::mat4(1.0), localScale);

	return localToWorld * trans;
}

glm::mat4 Transform::localTransformMatrix()
{

	glm::mat4 trans = glm::translate(glm::mat4(1.0f), localPosition) * localRotation.getMatrix() * glm::scale(glm::mat4(1.0), localScale);

	return trans;
}

glm::vec3 Transform::forward()
{
	return glm::normalize(worldForward * GetGlobalRotation());
}

glm::vec3 Transform::backward()
{
	return -forward();
}

glm::vec3 Transform::right()
{
	return -left();
}

glm::vec3 Transform::left()
{
	return glm::normalize((worldLeft * GetGlobalRotation()));
}

glm::vec3 Transform::up()
{
	return glm::normalize((worldUp * GetGlobalRotation()));
}

glm::vec3 Transform::down()
{
	return -up();
}

void Transform::SetLocalRotation(Quaternion rotation)
{
	_addToUpdateTransformList();
	localRotation.quaternion = rotation.quaternion;
}
