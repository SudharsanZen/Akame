#pragma once
#include<iostream>
#include"Physics/Physics.h"
#include"PxPhysicsAPI.h"
#include"Transform.h"
#include "snippetcommon/SnippetPrint.h"
#include "snippetcommon/SnippetPVD.h"

#ifndef _PHYSICS_HEADER_
#define _PHYSICS_HEADER_
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
		physx::PxDefaultErrorCallback gDefaultErrorCallback;
		physx::PxDefaultAllocator gDefaultAllocatorCallback;

		physx::PxFoundation* mFoundation = NULL;
		physx::PxPvd* mPvd = NULL;
		physx::PxPhysics* mPhysics = NULL;
		physx::PxDefaultCpuDispatcher* mDispatcher = NULL;
		physx::PxScene* mScene = NULL;
		physx::PxMaterial* mMaterial = NULL;
		physx::PxReal mAccumulator = 0.0f;

		friend class Scene;
		friend class RigidBodySystem;
		friend class RigidBody3D;
	public:
		Physics():Physics(1.0f/60.0f) {}
		Physics(physx::PxReal stepSize) { mStepSize = stepSize; }

		physx::PxReal mStepSize = 1.0f / 60.0f;
		
		template<typename T>
		void PX_RELEASE(T* x) { if (x)  x->release(); x = NULL; }

		void cleanPhysics();
		void initPhysics();



		bool advance(physx::PxReal dt);

	

		~Physics() { cleanPhysics(); }
	
	};
}

#endif