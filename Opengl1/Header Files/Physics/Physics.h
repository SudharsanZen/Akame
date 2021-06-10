#pragma once

#include"Transform.h"


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

	physx::PxTransform _ToPxTrans(Transform t);


	glm::vec3 _PxToVec3(physx::PxVec3 v);

	glm::quat _PxToQuat(physx::PxQuat q);

	Transform _PxToTrans(physx::PxTransform pxt);

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
		Physics():Physics(1.0f/60.0f) {}
		Physics(float stepSize) { mStepSize = stepSize; }

		float mStepSize = 1.0f / 60.0f;
		
		template<typename T>
		void PX_RELEASE(T* x) { if (x)  x->release(); x = NULL; }

		void cleanPhysics();
		void initPhysics();



		bool advance(float dt);

	

		~Physics() { cleanPhysics(); }
	
	};
}

#endif