#pragma once
#include"Math/EngineMathConstants.h"
#include"Math/Quaternion.h"
#include<list>
#include<algorithm>
#include"ECS.h"
class Transform
{
	EntityID parent=-1;
	EntityID current;
	std::list<EntityID> child;
public:
	glm::vec3 position;
	glm::vec3 scale;
	Quaternion rotation;

	
	std::weak_ptr<ECS> ecs;

	std::list<EntityID>& getChildList()
	{
		return child;
	}
	Transform& getParentTransform()
	{
		assert(parent != -1 && "trying to access non existing parent transform, this transform doesn't have any parent");
		return ecs.lock()->GetComponent<Transform>(parent);
	}

	void setParent(EntityID eid)
	{
		removeParent();
		
		//insert itself into the child list of the new parent
		parent = eid;
		Transform& p = ecs.lock()->GetComponent<Transform>(parent);
		p.child.push_back(current);
	}
	void removeParent()
	{
		//remove the current entity ID from it's current parent's child list
		if (parent != -1)
		{
			Transform& prevP = ecs.lock()->GetComponent<Transform>(parent);
			auto itr = std::find(prevP.child.begin(), prevP.child.end(), current);

			if (itr != prevP.child.end())
			{
				prevP.child.erase(itr);
			}
		}
	}
	void reset()
	{
		position = glm::vec3(0);
		scale = glm::vec3(1);
		rotation.setEulerAngle(0,0,0);
	}
	Transform();
	Transform(float posX,float posY,float posZ);
	Transform(glm::vec3 vec):Transform(vec.x,vec.y,vec.z) {};
	Transform& operator =(const Transform& Tran)
	{
		position = Tran.position;
		scale = Tran.scale;
		rotation.quaternion = Tran.rotation.quaternion;
		return *this;
	}
	glm::mat4 transformMatrix()
	{
		glm::mat4 trans = glm::mat4(1.0f);
		trans=glm::translate(trans,position);
		trans = trans * rotation.getMatrix();
		trans = glm::scale(trans, scale);
		return trans;
	}
	glm::vec3 forward()
	{
		return glm::normalize(rotation*worldForward);
	}
	glm::vec3 backward()
	{
		return -forward();
	}
	glm::vec3 right()
	{
		return -left();
	}
	glm::vec3 left()
	{
		return glm::normalize(rotation * worldLeft);
	}
	glm::vec3 up()
	{
		return glm::normalize(rotation * worldUp);
	}
	glm::vec3 down()
	{
		return -up();
	}
};