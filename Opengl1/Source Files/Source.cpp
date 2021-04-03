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
		
		float s =glm::dot(cam.transform.up(),worldUp);
		int sign = s/abs(s);
		Y += -(x - xPrev) * 0.5*(sign);
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
	const GLchar* vertexShaderDir = "..\\shaders\\InstancedShader\\vertexShaderInstanced.vert";
	const GLchar* fragmentShaderDir = "..\\shaders\\InstancedShader\\fragmentShaderInstanced.frag";
	const GLchar* lvs = "..\\shaders\\lightShader.vert";
	const GLchar* lfs = "..\\shaders\\lightShader.frag";

	Window window(800,800,"Main Window",NULL);
	

	if (!window.initialize())
	{
		std::cout <<"ERROR::could not initialize Window!";
		return -1;
	}

	Camera cam(60, 1, 1, 10000);
	cam.setFieldOfView(60.0f);
	

	glfwSwapInterval(0);
	
	
	Shader shader(vertexShaderDir,fragmentShaderDir);

	Shader shader2(lvs,lfs);
	Mesh sp2, light;
	light.createMesh<GLfloat, GLuint>(boxNormVertices, NULL, sizeof(boxNormVertices) / sizeof(boxNormVertices[0]), 0);
	
	std::vector<vert> spVec=sphere(32,32,1);
	std::vector<Transform> boxT;
	for (int i = 0; i < 100000; i++)
	{	
		int r = i / 316;
		int c = i % 316;
		float Zoff = 2, Xoff = 2;
		Transform t(c * Xoff, 0, Zoff * r);
		boxT.push_back(t);
	}
	//glEnable(GL_MULTISAMPLE); 
	MeshInstance boxes(boxT);
	boxes.createMesh<GLfloat,GLfloat>(boxNormVertices, NULL, sizeof(boxNormVertices) / sizeof(boxNormVertices[0]), 0);

	Texture tex("../Assets/container.jpg",GL_RGB);
	Texture tex1("../Assets/elephant1.png",GL_RGBA);

	tex.loadImage();
	tex1.loadImage();

	
	glEnable(GL_DEPTH_TEST);
	glClearColor(1,1,1,1);

	//Editor mainEditor(window);

	GLfloat t =0;
	float a= 1;
	

	glm::vec3 lightPose = glm::vec3(0, 2, -20);
	Transform lit;
	
	lit.scale=glm::vec3(0.2,0.2,0.2);

	GLdouble lt = 0;
	while (!window.shouldWindowClose())
	{
		float currTime = glfwGetTime();
		if (Input::getKeyDown(GLFW_KEY_1))
			lightPose.y -= 0.01f;
		if (Input::getKeyDown(GLFW_KEY_2))
			lightPose.y += 0.01f;
		
		lit.position = glm::vec3(lightPose.x, lightPose.y, lightPose.z);
		lt += deltaTime*5;
		for (int i=0; i < boxT.size(); i++)
		{
			float r = i / 316;
			float c = i % 316;
			float y = cos((c+lt)/10)*sin((r+lt)/10);
			
			boxT[i].position.y=y*10;
			
				boxT[i].rotation = boxT[i].rotation * Quaternion::rotationAroundAxisVector(10 * deltaTime, worldUp);

		}
		boxes.updateTransformBuffer();

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
				boxes.renderMesh();
			
			
				
				
			
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





