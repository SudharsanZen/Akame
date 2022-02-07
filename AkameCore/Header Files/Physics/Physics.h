#pragma once

#include"Components/Rendering/Transform.h"


#ifndef _PHYSICS_HEADER_
#define _PHYSICS_HEADER_
namespace physx
{
	class PxVec3;
	class PxQuat;
	class PxTransform;
	class PxFoundation;
	class PxPvd;
	class PxPhysics;
	class PxDefaultCpuDispatcher;
	class PxScene;
	class PxMaterial;
}
namespace physics
{
/* a bunch of functions to do conversions of orientational data from Physx to glm*/
	
	physx::PxVec3 _ToPxVec3(glm::vec3 v);

	physx::PxQuat _ToPxQuat(glm::quat q);

	physx::PxTransform _ToPxTrans(Transform &t);



	glm::vec3 _PxToVec3(physx::PxVec3 v);

	glm::quat _PxToQuat(physx::PxQuat q);

	void _PxToTrans(physx::PxTransform pxt, Transform& targetTransform);
	void _PxToTrans(physx::PxTransform pxt, Transform& targetTransform,float mix);

	/*class to handle physics*/
	class Physics
	{
	private:
		
		physx::PxFoundation* mFoundation = NULL;
		physx::PxPvd* mPvd = NULL;
		physx::PxPhysics* mPhysics = NULL;
		physx::PxDefaultCpuDispatcher* mDispatcher = NULL;
		physx::PxScene* mScene = NULL;
		physx::PxMaterial* mMaterial = NULL;
		float mAccumulator = 0.0f;

		friend class Scene;
		friend class RigidBodySystem;
		friend class RigidBody3D;
	public:
		AKAME_API Physics();
		AKAME_API Physics(float stepSize);

		float mStepSize;
		
		template<typename T>
		void PX_RELEASE(T* x) 
		{ 
			if (x)  
				x->release(); 
			x = NULL; 
		}

		AKAME_API void cleanPhysics();
		AKAME_API void initPhysics();

		AKAME_API bool isAdvancing(float dt);

		AKAME_API bool advance(float dt);

	

		AKAME_API ~Physics();
	
	};
}

#endif