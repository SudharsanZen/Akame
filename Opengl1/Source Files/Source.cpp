#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include"Engine.h"
#include"temp.h"
#include<vector>

void flyCam(Camera& cam)
{

	static double xPrev=0, yPrev=0;
	double x = 0, y = 0;
	static float X=0, Y=0;
	
	if (Input::getKeyDown(GLFW_KEY_W))
	{
		cam.setCameraPosition(cam.getCameraPosition() + cam.transform.forward() * 0.01f);

	}
	if (Input::getKeyDown(GLFW_KEY_S))
	{
		cam.setCameraPosition(cam.getCameraPosition() - cam.transform.forward() * 0.01f);
		
	}
	if (Input::getKeyDown(GLFW_KEY_A))
	{
		cam.setCameraPosition(cam.getCameraPosition() - cam.transform.right() * 0.01f);
		
	}
	if (Input::getKeyDown(GLFW_KEY_D))
	{
		cam.setCameraPosition(cam.getCameraPosition() + cam.transform.right() * 0.01f);

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

	Camera cam;
	cam.setFieldOfView(60.0f);
	

	glfwSwapInterval(0);
	
	
	Shader shader(vertexShaderDir,fragmentShaderDir);

	Shader shader2(lvs,lfs);
	Mesh box1,box2,box3,plane1,light,sp,sp2;
	std::vector<vert> spVec=sphere(32,32,1);

	//box.createMesh(bvertices, bIndex, sizeof(bvertices) / sizeof(bvertices[0]), sizeof(bIndex));
	box1.createMesh<GLfloat,GLuint>(boxNormVertices,NULL,sizeof(boxNormVertices)/sizeof(boxNormVertices[0]),0);
	box2.createMesh<GLfloat, GLuint>(boxNormVertices,NULL,sizeof(boxNormVertices)/sizeof(boxNormVertices[0]),0);
	box3.createMesh<GLfloat, GLuint>(boxNormVertices,NULL,sizeof(boxNormVertices)/sizeof(boxNormVertices[0]),0);
	light.createMesh<GLfloat, GLuint>(boxNormVertices,NULL,sizeof(boxNormVertices)/sizeof(boxNormVertices[0]),0);
	plane1.createMesh<GLfloat, GLuint>(plane,planeIndex,sizeof(plane)/sizeof(plane[0]),sizeof(planeIndex));
	sp2.createMesh<GLfloat, GLuint>(&spVec[0],NULL,spVec.size()*8,0);
	//sp.createMesh<GLfloat, GLuint>(&spVec[0],NULL,spVec.size()*sizeof(GLfloat)*8,0);
	//std::cout <<sizeof(boxNormVertices)/sizeof(boxNormVertices[0]) ;
	Texture tex("../Assets/container.jpg",GL_RGB);
	Texture tex1("../Assets/elephant1.png",GL_RGBA);
	Texture tex2("../Assets/earth3k.jpg", GL_RGB);
	tex.loadImage();
	tex1.loadImage();
	tex2.loadImage();
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(1,1,1,1);

	// Setup Dear ImGui context
	
	Editor mainEditor(window);

	GLfloat t =0;
	float a= 1;
	Transform transform1(-1,0.01,6);
	Transform transform2(1,0.01,6);
	Transform transform3(0,0.01,5);
	Transform transform4(0,-0.5,5);
	Transform spTrans(0,2,5);
	Transform lineT(0,0,0);
	glm::vec3 lightPose = glm::vec3(0, 2, -20);
	Transform lit;
	
	lit.scale=glm::vec3(0.2,0.2,0.2);
	transform4.scale = glm::vec3(1,1,1)*5.0f;
	transform4.rotation.setEulerAngle(90,0,0);

	glm::vec3 st = glm::vec3(0, 0, 0);
	glm::vec3 end = glm::vec3(0, 0, 10);
	
	lines l(st, end);

	while (!window.shouldWindowClose())
	{
		
		spTrans.rotation= spTrans.rotation*Quaternion::rotationAroundAxisVector(0.001f,worldUp);
		if (Input::getKeyDown(GLFW_KEY_1))
			lightPose.y -= 0.01f;
		if (Input::getKeyDown(GLFW_KEY_2))
			lightPose.y += 0.01f;
		//transform1.rotation.setEulerAngle(45,45,45);
		lit.position = glm::vec3(lightPose.x, lightPose.y, lightPose.z);
		transform3.rotation.quaternion =Quaternion::rotationAroundAxisVector(0.01f,worldUp)*transform3.rotation.quaternion;
		//transform3.rotation.quaternion =Quaternion::rotationAroundAxisVector(0.1f,transform3.right())* transform3.rotation.quaternion;
		glm::mat4 trans1 = transform1.transformMatrix();
		glm::mat4 trans2 = transform2.transformMatrix();
		glm::mat4 trans3 = transform3.transformMatrix();
		glm::mat4 trans4 = transform4.transformMatrix();
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
				shader.setUniformMat4fv("transform", 1, glm::value_ptr(trans4));
				plane1.renderMesh();
				shader.setUniformMat4fv("transform", 1, glm::value_ptr(spT));
			
				tex.use(0);
				tex2.use(1);
				sp2.renderMesh();
			
			shader2.useShaderProgram();
				shader2.setUniformMat4fv("transform", 1, glm::value_ptr(lightTrans));
				shader2.setUniformMat4fv("proj", 1, glm::value_ptr(proj));
				shader2.setUniformMat4fv("view", 1, glm::value_ptr(view));
				

				light.renderMesh();
				
				

			//mainEditor.DrawUI();
				
		
		window.processInput();
		window.swapBuffers();
		
	}


	return 0;
}





