#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include"Engine.h"
#include"temp.h"
#include<vector>

float deltaTime;
void flyCam(Camera& cam)
{

	static double xPrev=0, yPrev=0;
	double x = 0, y = 0;
	static float X=0, Y=0;
	float speed =10;
	if (Input::getKeyDown(GLFW_KEY_LEFT_SHIFT))
	{
		speed *=5;
	}
	if (Input::getKeyDown(GLFW_KEY_W))
	{
		cam.setCameraPosition(cam.getCameraPosition() + cam.transform.forward() * speed*deltaTime);

	}
	if (Input::getKeyDown(GLFW_KEY_S))
	{
		cam.setCameraPosition(cam.getCameraPosition() - cam.transform.forward() * speed * deltaTime);
		
	}
	if (Input::getKeyDown(GLFW_KEY_A))
	{
		cam.setCameraPosition(cam.getCameraPosition() - cam.transform.right() * speed * deltaTime);
		
	}
	if (Input::getKeyDown(GLFW_KEY_D))
	{
		cam.setCameraPosition(cam.getCameraPosition() + cam.transform.right() * speed * deltaTime);

	}
	Input::getMouseXY(x,y);

	
	
	if (Input::getMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		X += -(y - yPrev)*0.5;
		Y += -(x - xPrev)*0.5;
		cam.transform.rotation=Quaternion::rotationAroundAxisVector(Y,worldUp);
		cam.transform.rotation= Quaternion::rotationAroundAxisVector(X,cam.transform.right())* cam.transform.rotation.quaternion;
	}
	yPrev = y;
	xPrev = x;
}



int main()
{
	deltaTime = 0;
	float lastTime = 0;
	stbi_set_flip_vertically_on_load(true);
	const GLchar* vertexShaderDir = "..\\shaders\\vertexShader.vert";
	const GLchar* fragmentShaderDir = "..\\shaders\\fragmentShader.frag";
	const GLchar* lvs = "..\\shaders\\lightShader.vert";
	const GLchar* lfs = "..\\shaders\\lightShader.frag";

	Window window(800,800,"Main Window",NULL);
	

	if (!window.initialize())
	{
		std::cout <<"ERROR::could not initialize Window!";
		return -1;
	}

	Camera cam(60, 1, 1, 512);
	cam.setFieldOfView(60.0f);
	

	glfwSwapInterval(0);
	
	
	Shader shader(vertexShaderDir,fragmentShaderDir);

	Shader shader2(lvs,lfs);
	Mesh sp2, light;
	
	
	std::vector<vert> spVec=sphere(32,32,1);
	std::vector<Mesh*> boxes;
	std::vector<Transform> boxT;
	for (int i = 0; i < 10000; i++)
	{
		Mesh* box = new Mesh();
		box->createMesh<GLfloat, GLuint>(boxNormVertices, NULL, sizeof(boxNormVertices) / sizeof(boxNormVertices[0]), 0);
		//box->createMesh<GLfloat, GLuint>(&spVec[0], NULL, spVec.size() * 8, 0);
		boxes.push_back(box);
		int r = i / 100;
		int c = i % 100;
		float Zoff = 2, Xoff = 2;
		Transform t(c * Xoff, 0, Zoff * r);
		boxT.push_back(t);
	}

	light.createMesh<GLfloat, GLuint>(boxNormVertices,NULL,sizeof(boxNormVertices)/sizeof(boxNormVertices[0]),0);
	sp2.createMesh<GLfloat, GLuint>(&spVec[0],NULL,spVec.size()*8,0);
	//sp.createMesh<GLfloat, GLuint>(&spVec[0],NULL,spVec.size()*sizeof(GLfloat)*8,0);
	//std::cout <<sizeof(boxNormVertices)/sizeof(boxNormVertices[0]) ;
	Texture tex("../Assets/container.jpg",GL_RGB);
	Texture tex1("../Assets/elephant1.png",GL_RGBA);
	//Texture tex2("../Assets/earth3k.jpg", GL_RGB);
	tex.loadImage();
	tex1.loadImage();
	//tex2.loadImage();
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(1,1,1,1);

	// Setup Dear ImGui context
	
	Editor mainEditor(window);

	GLfloat t =0;
	float a= 1;
	
	Transform spTrans(0,2,5);
	Transform lineT(0,0,0);
	glm::vec3 lightPose = glm::vec3(0, 2, -20);
	Transform lit;
	
	lit.scale=glm::vec3(0.2,0.2,0.2);
	
	glm::vec3 st = glm::vec3(0, 0, 0);
	glm::vec3 end = glm::vec3(0, 0, 10);
	
	lines l(st, end);

	while (!window.shouldWindowClose())
	{
		float currTime = glfwGetTime();
		spTrans.rotation= spTrans.rotation*Quaternion::rotationAroundAxisVector(deltaTime*1.0f,worldUp);
		if (Input::getKeyDown(GLFW_KEY_1))
			lightPose.y -= 0.01f;
		if (Input::getKeyDown(GLFW_KEY_2))
			lightPose.y += 0.01f;
		//transform1.rotation.setEulerAngle(45,45,45);
		lit.position = glm::vec3(lightPose.x, lightPose.y, lightPose.z);

		//transform3.rotation.quaternion =Quaternion::rotationAroundAxisVector(0.1f,transform3.right())* transform3.rotation.quaternion;
		
		glm::mat4 spT = spTrans.transformMatrix();
		glm::mat4 ln = lineT.transformMatrix();
		glm::mat4 lightTrans = lit.transformMatrix();
		glm::mat4 proj = cam.getProjectionMatrix();
		glm::mat4 view = cam.getViewMatrix();
		cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		glfwPollEvents();
		
			//glClearColor(0.2f,0.3f,0.3f,1.0f);
			//glClearColor(1,1,1,1.0f);
			glClearColor(0,0,0,1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			flyCam(cam);
			
			shader.useShaderProgram();
				
				shader.setUniformVec3("lpose",lightPose);
				shader.setUniformVec3("viewPose",cam.getCameraPosition());
				shader.setUniformInteger("texture1",0);
				shader.setUniformInteger("texture2",1);
				shader.setUniformMat4fv("proj",1, glm::value_ptr(proj));
				shader.setUniformMat4fv("view",1, glm::value_ptr(view));
				tex.use(0);
				tex1.use(1);
				for (int i = 0; i < boxes.size(); i++)
				{
					//boxT[i].rotation=boxT[i].rotation*Quaternion::rotationAroundAxisVector(10*deltaTime,worldUp);
					glm::mat4 trans =boxT[i].transformMatrix();
					shader.setUniformMat4fv("transform",1,glm::value_ptr(trans));
					boxes[i]->renderMesh();
				}
				
				shader.setUniformMat4fv("transform", 1, glm::value_ptr(spT));
				sp2.renderMesh();
			
			
				
				
			
			shader2.useShaderProgram();
				shader2.setUniformMat4fv("transform", 1, glm::value_ptr(lightTrans));
				shader2.setUniformMat4fv("proj", 1, glm::value_ptr(proj));
				shader2.setUniformMat4fv("view", 1, glm::value_ptr(view));
				

				light.renderMesh();
				
				

			//mainEditor.DrawUI();
				
		
		window.processInput();
		window.swapBuffers();
		deltaTime = currTime - lastTime;
		lastTime = currTime;
	}


	return 0;
}





