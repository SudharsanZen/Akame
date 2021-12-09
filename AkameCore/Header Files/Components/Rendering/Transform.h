#pragma once
#include"Math/EngineMathConstants.h"
#include"Math/Quaternion.h"
#include<list>
#include<algorithm>
#include"ECS.h"
#include"Components/Components.h"
#include"Physics/System/RigidBodySystem.h"
class Transform:public Components
{
	glm::vec3 pxPoseInit;
	glm::quat pxRotInit;
	Entity parent=Entity(-1,-1);
	std::shared_ptr<std::set<e_index>> transformUpdateList;
	std::list<Entity> child;
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
	glm::mat4 transformMat;

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
		std::shared_ptr<ECS> E = ecs.lock();

		transformMat = transformMatrix();
		if (ecs.expired())
		{
			std::cout << "expired";
		}
		e_index transformIndex =E->GetComponentBitPose<Transform>();
		for (auto e : child)
		{
			Transform& t = E->GetComponent<Transform>((*e.componentIndex)[transformIndex],transformIndex);
			t.basePosition = GetGlobalPosition();
			t.baseRotation = GetGlobalRotation();
			t.baseScale = GetGlobalScale();

			//get conversion matrices 
			
			t.localToWorld = transformMatrix();
			t.worldToLocal = glm::inverse(t.localToWorld);
			t.updateChildBaseTransformDetails();
		}
	}
	friend class Camera;
	

	template <typename T>
	friend struct ComponentAllocator;
	friend class SceneTransformManager;
	friend class Editor;
	friend class Material;
	friend class RenderingSystem;
	template <typename T>
	friend class ComponentArray;
	friend class physics::RigidBodySystem;

	void _addToUpdateTransformList()
	{
		if(transformUpdateList)
			transformUpdateList->insert(componentIndex);
	}
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

	glm::mat4 getCalculatedTransform()
	{
		return transformMat;
	}
	Transform();
	Transform(float posX, float posY, float posZ);
	Transform(glm::vec3 vec) :Transform(vec.x, vec.y, vec.z) {};
	Transform(const Transform &obj)=default;
	Transform(Transform &&obj)=default;

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
		
		return transformMatrix()*glm::vec4(0,0,0,1);
	}
	Quaternion GetGlobalRotation()
	{
		return baseRotation * localRotation;
	}
	glm::vec3 GetGlobalScale()
	{
		return baseScale * localScale;
	}


	void SetLocalPosition(glm::vec3 position)
	{
		_addToUpdateTransformList();
		localPosition=position;
	}
	void SetLocalScale(glm::vec3 scale)
	{
		_addToUpdateTransformList();
		localScale=scale;
	}
	void SetLocalRotation(Quaternion rotation)
	{
		_addToUpdateTransformList();
		localRotation.quaternion=rotation.quaternion;
	}

	void SetGlobalPosition(glm::vec3 position)
	{
		_addToUpdateTransformList();
		localPosition=worldToLocal*glm::vec4(position,1);
	}
	void SetGlobalRotation(Quaternion rotation)
	{
		_addToUpdateTransformList();
		if (parent == Entity(-1,-1))
			localRotation = rotation.quaternion;
		else
		localRotation= glm::inverse(baseRotation.quaternion)* rotation.quaternion;
	}

	void SetGlobalScale(glm::vec3 scale)
	{
		_addToUpdateTransformList();
		localScale=scale/baseScale;
	}

	std::list<Entity>& getChildList()
	{
		return child;
	}

	Transform& getParentTransform()
	{
		assert(parent != Entity(-1,-1) && "trying to access non existing parent transform, this transform doesn't have any parent");
		return ecs.lock()->GetComponent<Transform>(parent);
	}

	Entity getParentId()
	{
		assert(parent != Entity(-1, -1) && "trying to access non existing parent transform, this transform doesn't have any parent");
		return parent;
	}

	void setParent(Entity parentEID)
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

		updateChildBaseTransformDetails();
		

	}
	void removeParent()
	{
		//remove the current entity ID from it's current parent's child list
		localPosition = GetGlobalPosition();
		localScale = GetGlobalScale();
		localRotation = GetGlobalRotation();

		if (parent != Entity(-1,-1))
		{
			Transform& prevP = ecs.lock()->GetComponent<Transform>(parent);
			auto itr = std::find(prevP.child.begin(), prevP.child.end(), eid);
			
			if (itr != prevP.child.end())
			{
				prevP.child.erase(itr);
			}
			
		}
		parent = Entity(-1,-1);
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
	

	glm::mat4 transformMatrix()
	{
		
		glm::mat4 trans=glm::translate(glm::mat4(1.0f),localPosition) * localRotation.getMatrix()* glm::scale(glm::mat4(1.0), localScale);

		return localToWorld* trans;
	}

	glm::mat4 localTransformMatrix()
	{

		glm::mat4 trans = glm::translate(glm::mat4(1.0f), localPosition) * localRotation.getMatrix() * glm::scale(glm::mat4(1.0), localScale);

		return trans;
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