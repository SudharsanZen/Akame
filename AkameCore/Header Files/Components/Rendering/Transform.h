#pragma once
#include"Math/EngineMathConstants.h"
#include"Math/Quaternion.h"
#include<list>
#include<algorithm>
#include"ECS.h"
#include"Components/Components.h"

class Transform:public Components
{
	EntityID parent=EntityID(-1,-1);

	std::list<EntityID> child;
	//global parent transform
	glm::vec3 basePosition;
	glm::vec3 baseScale;
	Quaternion baseRotation;
	//always local
	glm::vec3 localPosition;
	glm::vec3 localScale;
	Quaternion localRotation;

	glm::mat4 localToWorld;
	glm::mat4 worldToLocal;

	glm::mat4 formTransformMatrix(glm::vec3 position,Quaternion rotation,glm::vec3 scale)
	{
		
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, position);
		trans = trans * rotation.getMatrix();
		trans = glm::scale(trans, scale);
		return trans;
	}
	void updateChildBaseTransformDetails()
	{
		for (auto e : child)
		{
			Transform& t = ecs.lock()->GetComponent<Transform>(e);
			t.basePosition = GetGlobalPosition();
			t.baseRotation = GetGlobalRotation();
			t.baseScale = GetGlobalScale();

			//get conversion matrices 
			
			t.localToWorld = transformMatrix();
			t.worldToLocal = glm::inverse(t.localToWorld);
			t.updateChildBaseTransformDetails();
		}
	}
	friend class SceneTransformManager;
	friend class Editor;

	void destroyChildren()
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
	
public:
	
	glm::vec3 GetLocalPosition()
	{
		return localPosition;
	}
	glm::vec3 GetLocalScale()
	{
		return localScale;
	}
	Quaternion GetLocalRotation()
	{
		return localRotation;
	}

	glm::vec3 GetGlobalPosition()
	{
		return localToWorld*glm::vec4(localPosition,1);
	}
	Quaternion GetGlobalRotation()
	{
		return baseRotation* localRotation;
	}
	glm::vec3 GetGlobalScale()
	{
		return baseScale * localScale;
	}


	void SetLocalPosition(glm::vec3 position)
	{
		localPosition=position;
	}
	void SetLocalScale(glm::vec3 scale)
	{
		localScale=scale;
	}
	void SetLocalRotation(Quaternion rotation)
	{
		localRotation.quaternion=rotation.quaternion;
	}

	void SetGlobalPosition(glm::vec3 position)
	{
		localPosition=worldToLocal*glm::vec4(position,1);
	}
	void SetGlobalRotation(Quaternion rotation)
	{
		if (parent == EntityID(-1,-1))
			localRotation = rotation.quaternion;
		else
		localRotation= glm::inverse(baseRotation.quaternion)* rotation.quaternion;
	}

	void SetGlobalScale(glm::vec3 scale)
	{
		localScale=scale/baseScale;
	}

	std::list<EntityID>& getChildList()
	{
		return child;
	}

	Transform& getParentTransform()
	{
		assert(parent != EntityID(-1,-1) && "trying to access non existing parent transform, this transform doesn't have any parent");
		return ecs.lock()->GetComponent<Transform>(parent);
	}

	
	void setParent(EntityID parentEID)
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
		localPosition = worldToLocal * glm::vec4(globalPose,1);
		localRotation = glm::inverse(baseRotation.quaternion)* globalRotation.quaternion;
		localScale = globalScale / baseScale;

		//push this entity to the parent's child list
		
		updateChildBaseTransformDetails();
		

	}
	void removeParent()
	{
		//remove the current entity ID from it's current parent's child list
		localPosition = GetGlobalPosition();
		localScale = GetGlobalScale();
		localRotation = GetGlobalRotation();

		if (parent != EntityID(-1,-1))
		{
			Transform& prevP = ecs.lock()->GetComponent<Transform>(parent);
			auto itr = std::find(prevP.child.begin(), prevP.child.end(), eid);
			
			if (itr != prevP.child.end())
			{
				prevP.child.erase(itr);
			}
			
		}
		parent = EntityID(-1,-1);
		basePosition = glm::vec3(0);
		baseScale = glm::vec3(1);
		baseRotation = Quaternion(1, 0, 0, 0);
		localToWorld = glm::mat4(1.0f);
		worldToLocal = glm::mat4(1.0f);
		updateChildBaseTransformDetails();

	}
	void reset()
	{
		auto cChild = child;
		for (auto ch : cChild)
		{
			Transform &t=ecs.lock()->GetComponent<Transform>(ch);
			t.setParent(parent);
		}
		removeParent();
		
		basePosition = glm::vec3(0);
		baseScale = glm::vec3(1);
		baseRotation.setEulerAngle(0,0,0);
		localPosition = glm::vec3(0);
		localScale = glm::vec3(1);
		localRotation.setEulerAngle(0, 0, 0);
		localToWorld = glm::mat4(1.0f);
		worldToLocal = glm::mat4(1.0f);
	}
	Transform();
	Transform(float posX,float posY,float posZ);
	Transform(glm::vec3 vec):Transform(vec.x,vec.y,vec.z) {};
	Transform& operator =(const Transform& Tran)
	{
		parent = Tran.parent;
		eid = Tran.eid;
		ecs = Tran.ecs;
		localPosition = Tran.localPosition;
		localScale = Tran.localScale;
		localRotation.quaternion = Tran.localRotation.quaternion;
		basePosition = Tran.basePosition;
		baseScale = Tran.baseScale;
		baseRotation.quaternion = Tran.baseRotation.quaternion;
		localToWorld = Tran.localToWorld;
		worldToLocal = Tran.worldToLocal;
		child = Tran.child;
		return *this;
	}
	glm::mat4 transformMatrix()
	{
		glm::mat4 trans = glm::mat4(1.0f);
		trans=glm::translate(trans,localPosition);
		trans = trans * localRotation.getMatrix();
		trans = glm::scale(trans, localScale);
		return localToWorld* trans;
	}
	glm::vec3 forward()
	{
		return glm::normalize(worldForward* GetGlobalRotation());
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
		return glm::normalize((worldLeft*GetGlobalRotation()));
	}
	glm::vec3 up()
	{
		return glm::normalize((worldUp*GetGlobalRotation()));
	}
	glm::vec3 down()
	{
		return -up();
	}
};