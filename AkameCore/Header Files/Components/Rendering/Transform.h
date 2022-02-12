#pragma once
#include"Math/EngineMathConstants.h"
#include"Math/Quaternion.h"
#include<list>
#include<algorithm>
#include"ECS.h"
#include"Components/Components.h"
#include"Physics/System/RigidBodySystem.h"
#include"Core/Reflection/ReflectionUIHandler.h"
#include"Core/AkameCore.h"
#include"Components/Physics/RigidBody3D.h"
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
	/*
	AK_SERIALIZABLES
	(
		AK_ID(localPosition)
		AK_ID(localScale)
		AK_ID(localRotation)
		AK_ID_COMPX_LIST(child)
		AK_ID_COMPX(parent)
	)*/
	AKAME_API glm::mat4 formTransformMatrix(glm::vec3 position, Quaternion rotation, glm::vec3 scale);
	AKAME_API void updateChildBaseTransformDetails();
	
	friend class Camera;
	friend class physics::RigidBody3D;
	friend class ReflectionMeta;
	template <typename T>
	friend struct ComponentAllocator;
	friend class SceneTransformManager;
	friend class Editor;
	friend class Material;
	friend class RenderingSystem;
	friend class SceneHierarchyWindow;
	template <typename T>
	friend class ComponentArray;
	friend class physics::RigidBodySystem;
	friend class InspectorWindow;

	AKAME_API void _addToUpdateTransformList();
	AKAME_API void destroyChildren();

	AKAME_API glm::mat4 getCalculatedTransform();
	AKAME_API Transform();
	AKAME_API Transform(float posX, float posY, float posZ);
	AKAME_API Transform(glm::vec3 vec);;
	AKAME_API Transform(const Transform &obj)=default;
	AKAME_API Transform(Transform &&obj) noexcept =default;

public:
	
	AKAME_API glm::vec3 GetLocalPosition();
	AKAME_API glm::vec3 GetLocalScale();
	AKAME_API Quaternion GetLocalRotation();

	AKAME_API glm::vec3 GetGlobalPosition();
	AKAME_API Quaternion GetGlobalRotation();
	AKAME_API glm::vec3 GetGlobalScale();


	AKAME_API void SetLocalPosition(glm::vec3 position);
	AKAME_API void SetLocalScale(glm::vec3 scale);
	AKAME_API void SetLocalRotation(Quaternion rotation);

	AKAME_API void SetGlobalPosition(glm::vec3 position);
	AKAME_API void SetGlobalRotation(Quaternion rotation);

	AKAME_API void SetGlobalScale(glm::vec3 scale);

	AKAME_API std::list<Entity>& getChildList();

	AKAME_API Transform& getParentTransform();

	AKAME_API Entity getParentId();

	AKAME_API void setParent(Entity parentEID);
	AKAME_API void removeParent();
	AKAME_API void reset();
	

	AKAME_API glm::mat4 transformMatrix();

	AKAME_API glm::mat4 localTransformMatrix();
	AKAME_API glm::vec3 forward();
	AKAME_API glm::vec3 backward();
	AKAME_API glm::vec3 right();
	AKAME_API glm::vec3 left();
	AKAME_API glm::vec3 up();
	AKAME_API glm::vec3 down();
};