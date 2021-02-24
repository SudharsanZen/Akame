#include<iostream>
#include<fstream>
#include<string>

#include<glad\glad.h>
#include<GLFW\glfw3.h>

#include"EngineMath.h"

#include<imGui\backends\imgui_impl_glfw.h>
#include<imGui\backends\imgui_impl_opengl3.h>

#include<math.h>
#include"EngineMath.h""
#include"Window.h"
#include"Shader.h"
#include"Mesh.h"
#include"Texture.h"

GLfloat tvertices[] = { 
	-0.5,-0.5,0,	1,0,0,	0,0,
	0.5,-0.5,0,		0,1,0,	1,0,
	0,0.5,0 ,		0,0,1,	0.5,1
};
GLuint tIndex[] = { 0,1,2 };

float bvertices[] = {
	// positions          // colors           // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};
GLuint bIndex[] = { 0,1,2,2,3,0};



void accessUniform(GLuint pID)
{
	GLuint varLocation = glGetUniformLocation(pID,"color");
	float timeValue = glfwGetTime();
	float green = sin(timeValue)/2.0f+0.5f;
	glUniform4f(varLocation,0,green,0,0);
		
}

int main()
{
	
	const GLchar* vertexShaderDir = "..\\shaders\\vertexShader.vert";
	const GLchar* fragmentShaderDir = "..\\shaders\\fragmentShader.frag";

	Window window(800,800,"Main Window",NULL);
	

	if (!window.initialize())
	{
		std::cout <<"ERROR::could not initialize Window!";
		return -1;
	}
	//glfwSwapInterval(0);
	GLuint VBO, VAO;
	
	Shader shader(vertexShaderDir,fragmentShaderDir);
	shader.callAfterUseProgram = accessUniform;
	Mesh triangle;
	triangle.createMesh(bvertices,bIndex,sizeof(bvertices)/sizeof(bvertices[0]),sizeof(bIndex)/sizeof(bIndex[0]));
	stbi_set_flip_vertically_on_load(true);
	Texture tex("../Assets/container.jpg",GL_RGB);
	Texture tex1("../Assets/awesomeface.png",GL_RGBA);
	tex.loadImage();
	tex1.loadImage();
	

	glClearColor(1,1,1,1);

	

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();


	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window.mainWindow, true);
	ImGui_ImplOpenGL3_Init("#version 150");
	
	
	
	bool show_demo_window = true;
	bool show_another_window = false;
	GLfloat t =0;
	Transform transform;
	while (!window.shouldWindowClose())
	{
		if (t > 360)
			t = 0;
		else
			t+=1;
		
		transform.rotation.setEulerAngle(0,0,t);
		glm::mat4 RotationMat = transform.transformMatrix();
		


		glfwPollEvents();
		
			glClearColor(1,1,1,1);
			glClear(GL_COLOR_BUFFER_BIT);
		
			shader.useShaderProgram();
			
			shader.setUniformInteger("texture1",0);
			shader.setUniformInteger("texture2",1);
			shader.setUniformMat4fv("transform",1,glm::value_ptr(RotationMat));
			tex.use(0);
			tex1.use(1);
			
			
		
			triangle.renderMesh();
		
			

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Demo Window");
			ImGui::Text("Bababoye");
			ImGui::End();

			ImGui::Render();
			int display_w, display_h;
			glfwGetFramebufferSize(window.mainWindow, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			

			// Update and Render additional Platform Windows
			// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
			//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

			
			
		
		
		
		window.processInput();
		window.swapBuffers();
		
	}


	return 0;
}


