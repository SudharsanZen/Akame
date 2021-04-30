#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include"Engine.h"
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
	static bool init = false;
	static double xPrev=0, yPrev=0;
	double x = 0, y = 0;
	static float X=-45, Y=-135;
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
	if(init)
	Input::getMouseXY(x,y);
	
	if (Input::getMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS || !init)
	{
		init = true;
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

		
	
	Model model("Assets/backpack/backpack.obj");
	
	glGenBuffers(1,&uboMatrixBufferID);
	glBindBuffer(GL_UNIFORM_BUFFER,uboMatrixBufferID);
	glBufferData(GL_UNIFORM_BUFFER,2*mat4Size,NULL,GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER,0);
	glBindBufferRange(GL_UNIFORM_BUFFER,0,uboMatrixBufferID,0,2*mat4Size);

	Camera cam(60, 1, 0.1f, 1000);
	cam.setFieldOfView(60.0f);
	cam.transform.position = glm::vec3(5,5,5);

	Texture tex("Assets/backpack/roughness.jpg");
	
	//Editor mainEditor(window);

	Material m1;
	m1.setDiffuseMap("Assets/pbr/rust/diffuse.png");
	m1.setSpecularMap("Assets/pbr/rust/roughness.png");
	m1.setNormalMap("Assets/pbr/rust/normal.png");

	Material m2;
	m2.setDiffuseMap("Assets/backpack/diffuse.jpg");
	m2.setSpecularMap(tex);
	//m2.setNormalMap("Assets/pbr/rust/normal.png", GL_RGB);
	
	
	Mesh sp(generateSphereVertices(32, 32, 1),std::vector<GLuint>());
	Mesh box(generateCubeVertices(10,10),std::vector<GLuint>());
	Mesh plane(generatePlaneVertices(),std::vector<GLuint>());
	Transform spT(0,5,0);
	Transform boxT(0, 1, 0);
	Transform modelT(4,2, 0);
	boxT.scale *= 2;
	Transform planeT(0, 0, 0);
	planeT.scale *= 100;


	glm::vec3 lightPose = glm::vec3(10,10, 10);

	glfwSwapInterval(1);
	lines lz(glm::vec3(0,0,0), glm::vec3(0, 0, 2),glm::vec3(0,0,1));
	lines lx(glm::vec3(0, 0, 0), glm::vec3(2, 0, 0), glm::vec3(1,0,0));
	lines ly(glm::vec3(0, 0, 0), glm::vec3(0, 2, 0), glm::vec3(0,1,0));

	
	StopWatch st;
	while (!window.shouldWindowClose())
	{

		float currTime = glfwGetTime();
		flyCam(cam, window);
		
		
		
			glClearColor(0.2f,0.3f,0.3f,1.0f);
			//glClearColor(1,1,1,1.0f);
			glClearColor(0,0,0,1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			
			//mainrendering
			glEnable(GL_DEPTH_TEST);
			
			m1.use(boxT,lightPose,cam.transform.position);
			box.renderMesh();
			m2.use(modelT,lightPose,cam.transform.position);
			model.Draw();
			m1.use(spT, lightPose, cam.transform.position);
			sp.renderMesh();
			m1.use(planeT, lightPose, cam.transform.position);
			plane.renderMesh();

			//debug rendering
			glDisable(GL_DEPTH_TEST);

			lx.renderMesh();
			lz.renderMesh();
			ly.renderMesh();
			
		window.processInput();
		window.swapBuffers();
		glfwPollEvents();
		deltaTime = currTime - lastTime;
		lastTime = currTime;

	}


	return 0;
}





