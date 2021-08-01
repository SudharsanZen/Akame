
#include"Engine.h"
#include"Editor/Scene.h"
#include<glm/glm/gtx/vector_angle.hpp>

class Snake :public Behaviour
{
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
	EntityID head;
	EntityID apple;
	float snakeSize = 1;
	std::vector < std::pair<float, float>> freeIndices;
	std::vector<EntityID> tailList;
	Mesh ap;
	std::shared_ptr<Model> appleModel;

	EntityID createCube(Material &mat,glm::vec3 pose=glm::vec3(0.5,0,0.5))
	{
		EntityID body = CreateEntity();
		Transform cT(pose);
		Mesh cub;
		cT.scale *= snakeSize;
		cub.CreateMesh(generateCubeVertices());
		AddComponent<Transform>(body, cT);
		AddComponent<Mesh>(body, cub);
		AddComponent<Material>(body, mat);
		return body;
	}
	
public:

	Snake(Camera &cam,float snakeSize=1,int x=2,int y=2) :headMat("DEFERRED"),tailMat("DEFERRED"),appleMat("DEFERRED"), mainCam(cam)
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
		headMat.setValue("normalStrength", 0.1);
		headMat.setValue("specIntensity", 1);

		tailMat.setTexture2D("material.diffuseMap", rootDir + "Media/snakeBody.png");
		tailMat.setTexture2D("material.specularMap", rootDir + "Media/pbr/crate/roughness.jpg");
		tailMat.setTexture2D("material.normalMap", rootDir + "Media/pbr/crate/normal.jpg");
		tailMat.setValue("normalStrength", 0.1);
		tailMat.setValue("specIntensity", 1);
		appleMat.setTexture2D("material.diffuseMap", rootDir + "Media/Apple/Apple_BaseColor.png");
		appleMat.setTexture2D("material.specularMap", rootDir + "Media/Apple/Apple_Roughtness.png");
		appleMat.setTexture2D("material.normalMap", rootDir + "Media/Apple/Apple_Normal.png");
		appleMat.setValue("normalStrength", 1);
		appleMat.setValue("specIntensity", 1);
		
	
		

	}

	void OnStart()override
	{
		appleModel = std::make_shared<Model>(rootDir + "Media/Apple/apple.obj");
		head = createCube(headMat);

		for(int i=1;i<3;i++)
			tailList.push_back(createCube(tailMat,glm::vec3(0.5,0,(0.5f-1.0f)*(float)i)));

		
		apple = CreateEntity();
		Transform aT(1.5, 0, -1.5);
		aT.scale *= 0.01;
		
		AddComponent<Transform>(apple, aT);
		AddComponent<Mesh>(apple, appleModel->meshes[0]);
		AddComponent<Material>(apple, appleMat);
		Lights point(LIGHT::POINT);
		point.setColor(1,0.5,0);
		point.setPointLightConst(1,5,7);
		point.setIntensity(6);
		AddComponent<Lights>(apple,point);
		
		
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
		glm::vec3 newPose = ((glm::normalize(glm::vec3(0, 1, -1)) * 10.0f + headPose.position));
		if (gameOver)
			newPose.y = mainCam.transform.position.y;
		mainCam.transform.position += (-mainCam.transform.position + newPose) * deltaTime * 2.0f;
		mainCam.transform.rotation.setEulerAngle(-45, 0, 0);
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
		if (currTime > 0.1)
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

			
			headPose.position += moveDir;
			headPose.rotation = Quaternion::rotationAroundAxisVector(glm::orientedAngle(glm::vec3(0,0,1),moveDir,glm::vec3(0,1,0))*(180.0f/glm::pi<float>()),glm::vec3(0,1,0));
			currMoveDir = moveDir;
		}
		
		
		

		Transform &applePose=GetComponent<Transform>(apple);
		
		if (glm::length(applePose.position - headPose.position) <= 0.2f)
		{
			formFreeList();
			int randFreeListIndex = rand()%freeIndices.size();
			float randX = freeIndices[randFreeListIndex].first;
			float randZ = freeIndices[randFreeListIndex].second;

			if (score>=bX*bY*4)
			{
				applePose.position = glm::vec3(0, -2, 0);
				gameWon = true;
				return;
			}
			
			score += 1;
			
			applePose.position.x=randX;
			applePose.position.z=randZ;
			applePose.scale = glm::vec3(0.0f);
			tailList.push_back(createCube(tailMat,GetComponent<Transform>(tailList.back()).position));
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
			glm::vec3 pose = GetComponent<Transform>(tailList[j]).position;

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
		glm::vec3 pose = headPose.position;
		for (int i = 0; i < tailList.size(); i++)
		{
			glm::vec3 currtailPose = GetComponent<Transform>(tailList[i]).position;
			if (length(headPose.position - currtailPose) <= 0.2f)
				return true;
		}
		if (pose.z < (float)bY && pose.z > -(float)bY && pose.x < (float)bX && pose.x > -(float)bX)
			return false;
		
		
		
		return true;
	}
	float appleRot=0;
	void AppleBehv(float deltaTime)
	{
		glm::vec3 targetScale = glm::vec3(0.01f);
		Transform &appleT=GetComponent<Transform>(apple);
		appleT.scale += (targetScale - appleT.scale) * deltaTime*10.0f;
		appleRot += deltaTime*100;
		if(appleRot>360)
		appleRot = 0;
		appleT.rotation.setEulerAngle(0,appleRot,0);
	}
};