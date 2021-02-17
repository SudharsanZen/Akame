#include<iostream>
#include<fstream>
#include<string>

#include<glad\glad.h>
#include<GLFW\glfw3.h>
#include<imGui\backends\imgui_impl_glfw.h>
#include<imGui\backends\imgui_impl_opengl3.h>

#include<math.h>
#include"Window.h"
#include"Shader.h"
#include"Mesh.h"

GLfloat tvertices[] = { -0.5,-0.5,0,
						0.5,-0.5,0,
						0,0.5,0 };
GLuint tIndex[] = { 0,1,2 };

GLfloat bvertices[] = { 0.5,0.5,0,
						0.5,-0.5,0,
						-0.5,-0.5,0,
						-0.5,0.5,0};
GLuint bIndex[] = { 0,1,2,2,3,0};

int main()
{
	
	const GLchar* vertexShaderDir = "..\\shaders\\vertexShader.vert";
	const GLchar* fragmentShaderDir = "..\\shaders\\fragmentShader.frag";
	
	Window window(800,600,"Main Window",NULL);
	

	if (!window.initialize())
	{
		std::cout <<"ERROR::could not initialize Window!";
		return -1;
	}

	GLuint VBO, VAO;
	
	Shader shader(vertexShaderDir,fragmentShaderDir);
	Mesh triangle;
	triangle.createMesh(bvertices,bIndex,sizeof(bvertices)/sizeof(bvertices[0]),sizeof(bIndex)/sizeof(bIndex[0]));

	


	glClearColor(1,1,1,1);



	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window.mainWindow, true);
	ImGui_ImplOpenGL3_Init("#version 150");
	ImGui::StyleColorsDark();



	bool show_demo_window = true;
	bool show_another_window = false;
	
	while (!window.shouldWindowClose())
	{
		glfwPollEvents();
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);
			
	

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		
			shader.useShaderProgram();
			triangle.renderMesh();
			// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);

			ImGui::Begin("Demo Window");
			ImGui::End();

			// Rendering
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			
			
		
		
		window.processInput();
		window.swapBuffers();
		
	}


	return 0;
}


