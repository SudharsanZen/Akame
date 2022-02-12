#include"Physics\Physics.h"
#include<iostream>

#pragma warning(push, 0)
#pragma warning(disable : 26812)
#pragma warning(disable : 26495)
#pragma warning( disable : 26451)
#pragma warning( disable : 33010)
#include<PxPhysicsAPI.h>
#include <snippetcommon/SnippetPrint.h>
#include <snippetcommon/SnippetPVD.h>
#pragma warning(pop)
physx::PxDefaultErrorCallback gDefaultErrorCallback;
physx::PxDefaultAllocator gDefaultAllocatorCallback;

namespace physics 
{

	

	physx::PxVec3 _ToPxVec3(glm::vec3 v)
	{
		return physx::PxVec3(v.x, v.y, v.z);
	}
	physx::PxQuat _ToPxQuat(glm::quat q)
	{
		return physx::PxQuat(q.x, q.y, q.z, q.w);
	} 
	physx::PxTransform _ToPxTrans(Transform  &t)
	{
		return physx::PxTransform(_ToPxVec3(t.GetGlobalPosition()), _ToPxQuat(t.GetGlobalRotation().quaternion));
	}

	glm::vec3 _PxToVec3(physx::PxVec3 v)
	{
		return glm::vec3(v.x, v.y, v.z);
	}
	glm::quat _PxToQuat(physx::PxQuat q)
	{
		return glm::quat(q.w, q.x, q.y, q.z);
	}
	void _PxToTrans(physx::PxTransform pxt,Transform &targetTransform)
	{
		targetTransform.SetGlobalPosition(_PxToVec3(pxt.p));
		targetTransform.SetGlobalRotation(_PxToQuat(pxt.q));
	}

	void _PxToTrans(physx::PxTransform pxt, Transform& targetTransform, float mix)
	{

		glm::vec3 newPose=glm::mix(targetTransform.GetGlobalPosition(), _PxToVec3(pxt.p),mix);
		glm::quat newRot=glm::mix(targetTransform.GetGlobalRotation().quaternion, _PxToQuat(pxt.q),mix);
		targetTransform.SetGlobalPosition(newPose);
		targetTransform.SetGlobalRotation(newRot);
	}

	//initialize PhysX
	void Physics::initPhysics()
	{
		bool recordMemoryAllocations = true;

		mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
		if (!mFoundation)
		{
			std::cout << "Error, can't initialize mFoundation!\n";
			cleanPhysics();
			exit(-1);
		}


		mPvd = physx::PxCreatePvd(*mFoundation);
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
		mPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

		
		mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, physx::PxTolerancesScale(), recordMemoryAllocations, mPvd);
		if (!mPhysics)
		{
			std::cout << "Couldn,t create Physics object";
			cleanPhysics();
			exit(-1);
		}

		
		physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		mDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = mDispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		mScene = mPhysics->createScene(sceneDesc);
	
		mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);

		

		physx::PxPvdSceneClient* pvdClient = mScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}

		

	}

	Physics::~Physics() { cleanPhysics(); }

	bool Physics::isAdvancing(physx::PxReal dt)
	{
		
		if (mAccumulator + dt < mStepSize)
			return false;
		else
			return true;
	}

	//get's physics calculations after the step size (fixed time interval: default= (1.0f/60.0f) sec)
	bool Physics::advance(physx::PxReal dt)
	{
		mAccumulator += dt;
		if (mAccumulator < mStepSize)
			return false;

		mAccumulator -= mStepSize;

		mScene->simulate(mStepSize);
		return true;
	}

	Physics::Physics() :Physics(1.0f / 60.0f) 
	{

	}

	Physics::Physics(float stepSize)
	{ 
		mStepSize = stepSize; 
	}

	//releases all the pointers that has been allocated
	void Physics::cleanPhysics()
	{
		PX_RELEASE(mScene);
		PX_RELEASE(mPhysics);
		PX_RELEASE(mDispatcher);
		if (mPvd)
		{
			physx::PxPvdTransport* transport = mPvd->getTransport();
			mPvd->release();	
			mPvd = NULL;
			PX_RELEASE(transport);
		}
		PX_RELEASE(mFoundation);
	}
}