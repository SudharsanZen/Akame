
#include"Core/Engine.h"
#include"Core/Scene.h"
#include<glm/glm/gtx/vector_angle.hpp>

class Snake :public Behaviour
{
	Scene& scene;
	int bX,bY;
	bool gameWon=false;
	bool started=false;
	int score = 0;
	glm::vec3 moveDir;
	bool gameOver=false;
	glm::vec3 currMoveDir;
	Camera &mainCam;
	std::string rootDir;
	Material headMat;
	Material tailMat;
	Material appleMat;
	Entity head;
	Entity apple;
	float snakeSize = 1;
	std::vector < std::pair<float, float>> freeIndices;
	std::vector<Entity> tailList;
	Mesh ap;
	std::shared_ptr<Model> appleModel;

	Entity createCube(Material &mat,glm::vec3 pose=glm::vec3(0.5,0,0.5))
	{
		Entity body = CreateEntity();
		Transform cT(pose);
		AddComponent<Material>(body) = (mat);
		Mesh cub;
		cT.SetGlobalScale(glm::vec3(snakeSize));
		cub.CreateMesh(generateCubeVertices());
		AddComponent<Transform>(body)=cT;
		AddComponent<Mesh>(body)=cub;
		
		return body;
	}
	
public:

	Snake(Scene &s,Camera &cam,float snakeSize=1,int x=2,int y=2) :
		scene(s),
		headMat("DEFERRED"),
		tailMat("DEFERRED"),
		appleMat("DEFERRED"), 
		mainCam(cam)
	{
		bX = x;
		bY = y;
		this->snakeSize = snakeSize;
		moveDir=glm::vec3(0,0,1);
		currMoveDir=glm::vec3(0,0,1);
		rootDir = AssetManager::getAssetRoot();
		headMat.setTexture2D("material.diffuseMap", rootDir + "Media/snakeHead.png");
		headMat.setTexture2D("material.specularMap", rootDir + "Media/pbr/crate/roughness.jpg");
		headMat.setTexture2D("material.normalMap", rootDir + "Media/pbr/crate/normal.jpg");
		headMat.setValue("normalStrength", 0.1f);
		headMat.setValue("specIntensity", 1);

		tailMat.setTexture2D("material.diffuseMap", rootDir + "Media/snakeBody.png");
		tailMat.setTexture2D("material.specularMap", rootDir + "Media/pbr/crate/roughness.jpg");
		tailMat.setTexture2D("material.normalMap", rootDir + "Media/pbr/crate/normal.jpg");
		tailMat.setValue("normalStrength", 0.1f);
		tailMat.setValue("specIntensity", 1);
		appleMat.setTexture2D("material.diffuseMap", rootDir + "Media/Apple/Apple_BaseColor.png");
		appleMat.setTexture2D("material.specularMap", rootDir + "Media/Apple/Apple_Roughtness.png");
		appleMat.setTexture2D("material.normalMap", rootDir + "Media/Apple/Apple_Normal.png");
		appleMat.setValue("normalStrength", 1);
		appleMat.setValue("specIntensity", 1);
		
	
		

	}

	void OnStart()override
	{
		
		head = createCube(headMat);

		for(int i=1;i<3;i++)
			tailList.push_back(createCube(tailMat,glm::vec3(0.5f,0,(0.5f-1.0f)*(float)i)));

		
		apple = LoadModelToScene(scene,rootDir + "Media/Apple/apple.fbx");
		Transform aT(1.5, 0, -1.5);
		aT.SetGlobalScale(glm::vec3(0.001f));

		Lights point(LIGHT::POINT);
		point.setColor(1,0.5,0);
		point.setPointLightConst(1,5,7);
		point.setIntensity(6);
		AddComponent<Lights>(apple)=point;
		Transform &t=GetComponent<Transform>(apple);
		t.SetGlobalScale(glm::vec3(0.001));
		t.SetGlobalPosition(glm::vec3(1.5,0,-1.5));
		scene.SetEntityName(apple,"apple");
		
		
	}
	void Update(float deltaTime) override
	{
	
		AppleBehv(deltaTime);

		Transform& headPose = GetComponent<Transform>(head);

		glm::vec3 prevDir = moveDir;
		if (Input::getKeyDown(KEY_W))
		{
			moveDir = glm::vec3(0, 0, snakeSize);
			started = true;
		}
		if (Input::getKeyDown(KEY_S))
		{
			moveDir = glm::vec3(0, 0, -snakeSize);
			started = true;
		}
		if (Input::getKeyDown(KEY_D))
		{
			started = true;
			moveDir = glm::vec3(-snakeSize, 0, 0);
		}
		if (Input::getKeyDown(KEY_A))
		{
			started = true;
			moveDir = glm::vec3(snakeSize, 0, 0);
		}
		glm::vec3 newPose = ((glm::normalize(glm::vec3(0, 1, -1)) * 10.0f + headPose.GetGlobalPosition()));
		if (gameOver)
			newPose.y = mainCam.transform.GetGlobalPosition().y;
		mainCam.transform.SetGlobalPosition(mainCam.transform.GetGlobalPosition()+(-mainCam.transform.GetGlobalPosition() + newPose) * deltaTime * 2.0f);
		mainCam.transform.SetGlobalRotation(Quaternion(-45, 0, 0));
		if (!started)
			return;
		if (prevDir==-moveDir || moveDir==-currMoveDir)
			moveDir = prevDir;

		if (gameWon && hitBoundaries())
			return;
		if(!gameOver)
		gameOver = hitBoundaries();
		if (gameOver)
			moveDir = glm::vec3(0, -1, 0);
		moveSnake(deltaTime);
		
	}
	float currTime=0;
	void moveSnake(float deltaTime)
	{
		Transform& headPose = GetComponent<Transform>(head);
		currTime += deltaTime;
		if (currTime > 0.6)
		{
			currTime = 0;
			//update tail pose
			Transform prevPose = headPose;

			for (long int i = 0; i < tailList.size(); i++)
			{
				Transform temp;
				Transform& pose = GetComponent<Transform>(tailList[i]);
				temp = pose;
				pose = prevPose;
				prevPose = temp;
				
			}

			
			headPose.SetGlobalPosition(headPose.GetGlobalPosition()+moveDir);
			headPose.SetGlobalRotation(Quaternion::rotationAroundAxisVector(glm::orientedAngle(glm::vec3(0,0,1),moveDir,glm::vec3(0,1,0))*(180.0f/glm::pi<float>()),glm::vec3(0,1,0)));
			currMoveDir = moveDir;
		}
		
		
		

		Transform &applePose=GetComponent<Transform>(apple);
		
		if (glm::length(applePose.GetGlobalPosition() - headPose.GetGlobalPosition()) <= 0.2f)
		{


			
			formFreeList();
			int randFreeListIndex = rand()%freeIndices.size();
			float randX = freeIndices[randFreeListIndex].first;
			float randZ = freeIndices[randFreeListIndex].second;

			if (score>=bX*bY*4)
			{
				applePose.SetGlobalPosition(glm::vec3(0, -2, 0));
				gameWon = true;
				return;
			}
			
			score += 1;
			
			applePose.SetGlobalPosition(glm::vec3(randX,applePose.GetGlobalPosition().y,randZ));
			applePose.SetGlobalScale(glm::vec3(0.0f));
			tailList.push_back(createCube(tailMat,GetComponent<Transform>(tailList.back()).GetGlobalPosition()));
		}



	}

	void formFreeList()
	{
		//create a free list which contains locations which can be used to spawn food
		freeIndices.clear();
		for (int i = 0; i < bX*2; i++)
		{
			for (int j = 0; j < bY * 2; j++)
			{
				freeIndices.push_back(std::make_pair<float, float>(((float)i) - bX+0.5f, ((float)j)-bY+0.5f));
			}
		}

		for (int j = 0; j < tailList.size(); j++)
		{
			glm::vec3 pose = GetComponent<Transform>(tailList[j]).GetGlobalPosition();

			for (int i = 0; i < freeIndices.size(); i++)
			{
				if (pose.x == freeIndices[i].first && pose.z == freeIndices[i].second)
				{
					freeIndices.erase(freeIndices.begin()+i);
					break;
				}
			}
		}
		
			
		
	}


	bool hitBoundaries()
	{
		
		Transform &headPose = GetComponent<Transform>(head);
		glm::vec3 pose = headPose.GetGlobalPosition();
		for (int i = 0; i < tailList.size(); i++)
		{
			glm::vec3 currtailPose = GetComponent<Transform>(tailList[i]).GetGlobalPosition();
			if (length(headPose.GetGlobalPosition() - currtailPose) <= 0.2f)
				return true;
		}
		if (pose.z < (float)bY && pose.z > -(float)bY && pose.x < (float)bX && pose.x > -(float)bX)
			return false;
		
		
		
		return true;
	}
	float appleRot=0;
	void AppleBehv(float deltaTime)
	{
		glm::vec3 targetScale = glm::vec3(0.025f);
		Transform &appleT=GetComponent<Transform>(apple);
		appleT.SetGlobalScale(appleT.GetGlobalScale()+(targetScale - appleT.GetGlobalScale()) * deltaTime*10.0f);
		appleRot += 100* deltaTime;
		if(appleRot>360)
		appleRot = 0;
		appleT.SetGlobalRotation(Quaternion(0,appleRot,0));
	}
};
