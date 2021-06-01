#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include"Engine.h"
#include"Editor/Scene.h"
#include"PxPhysicsAPI.h"
#include "snippetcommon/SnippetPrint.h"
#include "snippetcommon/SnippetPVD.h"
#include<vector>
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}



static physx::PxDefaultErrorCallback gDefaultErrorCallback;
static physx::PxDefaultAllocator gDefaultAllocatorCallback;

physx::PxFoundation* mFoundation = NULL;
physx::PxPvd* mPvd = NULL;
physx::PxPhysics* mPhysics=NULL;
physx::PxDefaultCpuDispatcher* mDispatcher = NULL;
physx::PxScene* mScene = NULL;
physx::PxMaterial* mMaterial = NULL;

physx::PxRigidDynamic* createDynamic(const physx::PxTransform & t, const physx::PxGeometry & geometry, const physx::PxVec3 & velocity = physx::PxVec3(0))
{
	physx::PxRigidDynamic* dynamic = PxCreateDynamic(*mPhysics, t, geometry, *mMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	mScene->addActor(*dynamic);
	return dynamic;
}




void cleanPhysics();
void initPhysics();
physx::PxReal mAccumulator = 0.0f;
physx::PxReal mStepSize = 1.0f / 60.0f;

bool advance(physx::PxReal dt)
{
	mAccumulator += dt;
	if (mAccumulator < mStepSize)
		return false;

	mAccumulator -= mStepSize;

	mScene->simulate(mStepSize);
	return true;
}

int main()
{
	
	Window window(1920,1080,"testWindow");
	
	if (!window.initialize())
	{
		std::cout <<"Can't init WINDOW";
		return -1;
	}
	Scene scene(window);

	initPhysics();

	EntityID plane = scene.CreateEntity();
	EntityID box =scene.CreateEntity();
	EntityID box1 =scene.CreateEntity();
	
	Material boxMat;
	boxMat.setDiffuseMap("Assets/pbr/box/diffuse.png");
	boxMat.setSpecularMap("Assets/pbr/box/roughness.png");

	scene.AddComponent<Mesh>(box,Mesh());
	scene.GetComponent<Mesh>(box).CreateMesh(generateCubeVertices());
	scene.AddComponent<Transform>(box,Transform(0,0.5,0));
	scene.AddComponent<Material>(box,boxMat);


	scene.AddComponent<Mesh>(box1, Mesh());
	scene.GetComponent<Mesh>(box1).CreateMesh(generateCubeVertices());
	scene.AddComponent<Transform>(box1, Transform(0, 2, 0));
	scene.AddComponent<Material>(box1, boxMat);

	Transform planeTransform;

	planeTransform.scale*=5.0f;

	scene.AddComponent<Mesh>(plane, Mesh());
	scene.GetComponent<Mesh>(plane).CreateMesh(generatePlaneVertices());
	scene.AddComponent<Transform>(plane, planeTransform);
	scene.AddComponent<Material>(plane, boxMat);

	physx::PxShape* boxShape = mPhysics->createShape(physx::PxBoxGeometry(0.5, 0.5, 0.5), *mMaterial);

	physx::PxTransform pxBoxTransform(physx::PxVec3(0,5,0.5f));
	physx::PxRigidDynamic* pxBox = mPhysics->createRigidDynamic(pxBoxTransform);
	pxBox->attachShape(*boxShape);
	physx::PxRigidBodyExt::updateMassAndInertia(*pxBox, 1.0f);
	mScene->addActor(*pxBox);

	physx::PxTransform pxBox1Transform(physx::PxVec3(0, 2, 0));
	physx::PxRigidDynamic* pxBox1 = mPhysics->createRigidDynamic(pxBox1Transform);
	pxBox1->attachShape(*boxShape);
	physx::PxRigidBodyExt::updateMassAndInertia(*pxBox1, 1.0f);
	mScene->addActor(*pxBox1);

	physx::PxRigidStatic* groundPlane = physx::PxCreatePlane(*mPhysics, physx::PxPlane(0, 1, 0, 0), *mMaterial);
	mScene->addActor(*groundPlane);

	boxShape->release();

	glfwSwapInterval(1);
	scene.OnStart();
	scene.backGroundColor(0,0,0,1);
	while (!window.closeWindow())
	{
		
		flyCam(scene.cam,scene.getDeltaTime());
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());

		if(advance(scene.getDeltaTime()))
			mScene->fetchResults(true);

		Transform &boxT=scene.GetComponent<Transform>(box);
		physx::PxTransform pxBoxT = pxBox->getGlobalPose();
		boxT.position = glm::vec3(pxBoxT.p.x,pxBoxT.p.y,pxBoxT.p.z);
		
		boxT.rotation.quaternion = glm::quat(pxBoxT.q.x,pxBoxT.q.y,pxBoxT.q.z,pxBoxT.q.w);

		Transform& boxT1 = scene.GetComponent<Transform>(box1);
		physx::PxTransform pxBoxT1 = pxBox1->getGlobalPose();
		boxT1.position = glm::vec3(pxBoxT1.p.x, pxBoxT1.p.y, pxBoxT1.p.z);
		boxT1.rotation.quaternion = glm::quat(pxBoxT1.q.x, pxBoxT1.q.y, pxBoxT1.q.z, pxBoxT1.q.w);

		scene.Render();

		
	}
	//cleanPhysics();
	return 0;
}

void initPhysics()
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
	mMaterial = mPhysics->createMaterial(0.5f,0.5f,0.6f);

}

void cleanPhysics()
{

	PX_RELEASE(mFoundation);
	PX_RELEASE(mPhysics);
	PX_RELEASE(mDispatcher);
}



