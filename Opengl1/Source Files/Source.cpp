#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include"Engine.h"
#include"Editor/Scene.h"

#include<vector>




int main()
{
	
	Window window(1920,1080,"testWindow");
	
	if (!window.initialize())
	{
		std::cout <<"Can't init WINDOW";
		return -1;
	}
	Scene scene(window);



	EntityID plane = scene.CreateEntity();
	EntityID box =scene.CreateEntity();
	EntityID box1 =scene.CreateEntity();
	EntityID sphere =scene.CreateEntity();
	
	Material boxMat;
	boxMat.setDiffuseMap("Assets/5.jpg");
	boxMat.setSpecularMap("Assets/5.jpg");

	Material planeMat;
	planeMat.setDiffuseMap("Assets/pbr/rust/diffuse.png");
	planeMat.setSpecularMap("Assets/pbr/rust/roughness.png");

	scene.AddComponent<Mesh>(box,Mesh());
	scene.GetComponent<Mesh>(box).CreateMesh(generateCubeVertices());
	scene.AddComponent<Transform>(box,Transform(1,2,0));
	scene.AddComponent<Material>(box,boxMat);
	scene.GetComponent<Transform>(box).rotation.setEulerAngle(0, 0, 0);
	scene.AddComponent<physics::RigidBody3D>(box,physics::RigidBody3D());
	physics::RigidBody3D &rbdy=scene.GetComponent<physics::RigidBody3D>(box);
	rbdy.setRigidBodyType(physics::DYNAMIC,physics::BOX);

	scene.AddComponent<Mesh>(box1, Mesh());
	scene.GetComponent<Mesh>(box1).CreateMesh(generateCubeVertices());
	scene.AddComponent<Transform>(box1, Transform(0, 0.5, 0));
	scene.AddComponent<Material>(box1, boxMat);
	scene.AddComponent<physics::RigidBody3D>(box1, physics::RigidBody3D());
	physics::RigidBody3D& rbdy1 = scene.GetComponent<physics::RigidBody3D>(box1);
	rbdy1.setRigidBodyType(physics::DYNAMIC, physics::BOX);
	Transform planeTransform;

	planeTransform.scale*=5.0f;

	scene.AddComponent<Mesh>(plane, Mesh());
	scene.GetComponent<Mesh>(plane).CreateMesh(generatePlaneVertices());
	scene.AddComponent<Transform>(plane, planeTransform);
	scene.AddComponent<Material>(plane, planeMat);
	//scene.GetComponent<Transform>(plane).rotation.setEulerAngle(0,0,45);
	scene.AddComponent<physics::RigidBody3D>(plane, physics::RigidBody3D());
	physics::RigidBody3D& rbdy2 = scene.GetComponent<physics::RigidBody3D>(plane);
	physics::ColliderShape planeShape;
	planeShape.setColliderShape(physics::PLANE,5,5);
	rbdy2.setRigidBodyType(physics::STATIC,planeShape);
	

	
	glfwSwapInterval(1);

	scene.OnStart();
	scene.backGroundColor(0,0,0,1);
	while (!window.closeWindow())
	{
		
		flyCam(scene.cam,scene.getDeltaTime());
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		scene.Render();

		
	}
	//cleanPhysics();
	return 0;
}





