#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include"Engine.h"
#include"temp.h"
#include"time.h"

#include"stopwatch.h"
#include<vector>
GLuint uboMatrixBufferID;
GLsizeiptr mat4Size = sizeof(glm::mat4);
float deltaTime;
void updateUniformBuffer(Camera& cam)
{
	glm::mat4 projMat = cam.getProjectionMatrix();
	glm::mat4 viewMat = cam.getViewMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrixBufferID);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, mat4Size, glm::value_ptr(projMat));
	glBufferSubData(GL_UNIFORM_BUFFER, mat4Size, mat4Size, glm::value_ptr(viewMat));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

}
void flyCam(Camera& cam, Window &window)
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
		X += -(y - yPrev) * 0.5;
		Y += -(x - xPrev) * 0.5;
		//if (abs(Y) >= 360.0f)
			//Y =Y+((Y>0)?-1:1)*360.0f;
		Y = abs(Y) >= 360.0f ? 0 : Y;
		X = abs(X) >= 360.0f ? 0 : X;
		cam.transform.rotation = Quaternion::rotationAroundAxisVector(Y, worldUp);
		cam.transform.rotation = Quaternion::rotationAroundAxisVector(X, cam.transform.right()) * cam.transform.rotation.quaternion;

	
	}
	updateUniformBuffer(cam);
	cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
	yPrev = y;
	xPrev = x;
}



int main()
{
	deltaTime = 0;
	float lastTime = 0;
	stbi_set_flip_vertically_on_load(true);
	
	Window window(800,800,"Main Window",NULL);
	

	if (!window.initialize())
	{
		std::cout <<"ERROR::could not initialize Window!";
		return -1;
	}

		
	glEnable(GL_DEPTH_TEST);

	
	glGenBuffers(1,&uboMatrixBufferID);
	glBindBuffer(GL_UNIFORM_BUFFER,uboMatrixBufferID);
	glBufferData(GL_UNIFORM_BUFFER,2*mat4Size,NULL,GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER,0);
	glBindBufferRange(GL_UNIFORM_BUFFER,0,uboMatrixBufferID,0,2*mat4Size);

	Camera cam(60, 1, 1, 10000);
	cam.setFieldOfView(60.0f);
	
	//Editor mainEditor(window);

	Material m1;
	m1.setDiffuseMap("../Assets/pbr/box/diffuse.png", GL_RGBA);
	m1.setSpecularMap("../Assets/pbr/box/roughness.png", GL_RGBA);
	//m1.setNormalMap("../Assets/pbr/normal.jpg", GL_RGB);

	Mesh sp2, light;
	light.createMesh<GLfloat, GLuint>(boxNormVertices, NULL, sizeof(boxNormVertices) / sizeof(boxNormVertices[0]), 0);

	std::vector<vert> spVec = sphere(32, 32, 1);
	
	Mesh box;
	box.createMesh<GLfloat,GLfloat>(boxNormVertices, NULL, sizeof(boxNormVertices) / sizeof(boxNormVertices[0]), 0);
	//box.createMesh<GLfloat, GLfloat>(&spVec[0], NULL, spVec.size() * 8, 0);

	Transform boxT(0, 0, 5);


	glm::vec3 lightPose = glm::vec3(2,2, -2);
	Transform lit(lightPose.x,lightPose.y,lightPose.z);
	lit.scale=glm::vec3(0.2,0.2,0.2);


	glfwSwapInterval(0);


	while (!window.shouldWindowClose())
	{

		float currTime = glfwGetTime();
		flyCam(cam, window);

		glfwPollEvents();
		
			glClearColor(0.2f,0.3f,0.3f,1.0f);
			//glClearColor(1,1,1,1.0f);
			//glClearColor(0,0,0,1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			m1.use(boxT,lightPose,cam.transform.position);
			box.renderMesh();

			m1.use(lit, lightPose, cam.transform.position);
			light.renderMesh();
		window.processInput();
		window.swapBuffers();

		deltaTime = currTime - lastTime;
		lastTime = currTime;

	}


	return 0;
}





