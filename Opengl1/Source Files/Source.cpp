#include<iostream>
#include<fstream>
#include<string>
#include"Engine.h"
#include<algorithm>


GLfloat tvertices[] = { 
	-0.5,-0.5,0,		0,0,
	0.5,-0.5,0,			1,0,
	0,0.5,0 ,			0.5,1
};
GLuint tIndex[] = { 0,1,2 };

float bverticesCol[] = {
	// positions          // colors           // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

float bvertices[] = {
	// positions            // texture coords
	 0.5f,  0.5f, 0.0f,      1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,     1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,     0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,      0.0f, 1.0f    // top left 
};

GLuint bIndex[] = { 0,1,2,2,3,0};

float boxvertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

template <typename T>
T clamp(T a,T min,T max)
{
	if (a < min)
		a = min;
	if (a > max)
		a = max;
	return a;
}

void camTrans(Camera& cam)
{

	static double xPrev=0, yPrev=0;
	double x = 0, y = 0;
	static float X=0, Y=0;
	
	if (Input::getKeyDown(GLFW_KEY_W))
	{
		cam.setCameraPosition(cam.getCameraPosition() + cam.getLookDir() * 0.01f);
		cam.setCameraTargetPosition(cam.getCameraTargetPosition() + cam.getLookDir() * 0.01f);
	}
	if (Input::getKeyDown(GLFW_KEY_S))
	{
		cam.setCameraPosition(cam.getCameraPosition() - cam.getLookDir() * 0.01f);
		cam.setCameraTargetPosition(cam.getCameraTargetPosition() - cam.getLookDir() * 0.01f);
	}
	if (Input::getKeyDown(GLFW_KEY_A))
	{
		cam.setCameraPosition(cam.getCameraPosition() - cam.lookDirRight() * 0.01f);
		cam.setCameraTargetPosition(cam.getCameraTargetPosition() - cam.lookDirRight() * 0.01f);
	}
	if (Input::getKeyDown(GLFW_KEY_D))
	{
		cam.setCameraPosition(cam.getCameraPosition() + cam.lookDirRight() * 0.01f);
		cam.setCameraTargetPosition(cam.getCameraTargetPosition() + cam.lookDirRight() * 0.01f);
	}
	Input::getMouseXY(x,y);
	//cam.transform.rotation.setEulerAngle(0, 20, 0);

	//X=clamp(X,-90.0f,90.0f);
	if (Input::getMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		cam.transform.rotation.quaternion =glm::angleAxis((float)(x-xPrev)*0.01f,cam.lookDirUp()) ;
		cam.transform.rotation.quaternion *=glm::angleAxis((float)(y-yPrev)*0.01f,cam.lookDirRight()) ;
		glm::vec3 camPos = cam.getCameraPosition();
		glm::vec3 tPos = cam.getCameraTargetPosition()-camPos;
		tPos = cam.transform.rotation*tPos;
		cam.setCameraTargetPosition(camPos+tPos);
	}
	yPrev = y;
	xPrev = x;
}

int main()
{

	stbi_set_flip_vertically_on_load(true);
	const GLchar* vertexShaderDir = "..\\shaders\\vertexShader.vert";
	const GLchar* fragmentShaderDir = "..\\shaders\\fragmentShader.frag";

	Window window(800,800,"Main Window",NULL);
	

	if (!window.initialize())
	{
		std::cout <<"ERROR::could not initialize Window!";
		return -1;
	}
	Camera cam;


	cam.setFieldOfView(60.0f);
	cam.setCameraPosition(0,0,0);
	glfwSwapInterval(0);
	
	
	Shader shader(vertexShaderDir,fragmentShaderDir);
	Mesh box1,box2,box3;
	//box.createMesh(bvertices, bIndex, sizeof(bvertices) / sizeof(bvertices[0]), sizeof(bIndex));
	box1.createMesh(boxvertices,NULL,sizeof(boxvertices)/sizeof(boxvertices[0]),0);
	box2.createMesh(boxvertices,NULL,sizeof(boxvertices)/sizeof(boxvertices[0]),0);
	box3.createMesh(boxvertices,NULL,sizeof(boxvertices)/sizeof(boxvertices[0]),0);
	
	Texture tex("../Assets/container.jpg",GL_RGB);
	Texture tex1("../Assets/elephant1.png",GL_RGBA);
	tex.loadImage();
	tex1.loadImage();
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(1,1,1,1);

	// Setup Dear ImGui context
	
	Editor mainEditor(window);

	GLfloat t =0;
	float a= 1;
	Transform transform1(-1,0,6);
	Transform transform2(1,0,6);
	Transform transform3(0,0,5);
	cam.setCameraTargetPosition(0,0,5);

	while (!window.shouldWindowClose())
	{
		transform1.rotation.setEulerAngle(45,45,45);
		
		
		glm::mat4 trans1 = transform1.transformMatrix();
		glm::mat4 trans2 = transform2.transformMatrix();
		glm::mat4 trans3 = transform3.transformMatrix();
		glm::mat4 proj = cam.getProjectionMatrix();
		glm::mat4 view = cam.getViewMatrix();
		cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		glfwPollEvents();
		
			//glClearColor(0.2f,0.3f,0.3f,1.0f);
			glClearColor(1,1,1,1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			camTrans(cam);
			shader.useShaderProgram();
				shader.setUniformInteger("texture1",0);
				shader.setUniformInteger("texture2",1);
				shader.setUniformMat4fv("transform",1,glm::value_ptr(trans1));
				shader.setUniformMat4fv("proj",1, glm::value_ptr(proj));
				shader.setUniformMat4fv("view",1, glm::value_ptr(view));
				
				tex.use(0);
				tex1.use(1);
			box1.renderMesh();
			shader.setUniformMat4fv("transform", 1, glm::value_ptr(trans2));
			box2.renderMesh();
			shader.setUniformMat4fv("transform", 1, glm::value_ptr(trans3));
			box3.renderMesh();



			//mainEditor.DrawUI();
		
		
		window.processInput();
		window.swapBuffers();
		
	}


	return 0;
}





