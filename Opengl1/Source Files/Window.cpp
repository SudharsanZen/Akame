#include<iostream>
#ifdef _MSC_VER
#define NOMINMAX
#include <windows.h>
#endif // _MSC_VER
#include "window.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>

Window::Window(int width,int height,std::string winName,Window *shareWindow)
{
	
	/*constructor*/
	bufferHeight = 0;
	bufferWidth = 0;
	mainWindow = NULL;
	Height = height;
	Width = width;
	windowName = winName;
	share = shareWindow;
}

Window::~Window()
{
	glfwTerminate();
}

int Window::getBufferHeight()
{
	//get windows buffer Height
	if (mainWindow)
		glfwGetFramebufferSize(mainWindow.get(), &bufferWidth, &bufferHeight);
	else
		std::cout << "window ptr has been deleted or has not been initialized!\n";
	return bufferHeight;
}

int Window::getBufferWidth()
{
	//get windows buffer width
	if (mainWindow)
		glfwGetFramebufferSize(mainWindow.get(), &bufferWidth, &bufferHeight);
	else
		std::cout << "window ptr has been destroyed or has not been intialized!\n";
	return bufferWidth;
}

bool Window::closeWindow()
{
	//returns true if windows close event has occured
	if (mainWindow)
		return glfwWindowShouldClose(mainWindow.get());
	else
	{
		std::cerr << "ERROR::initialize window first!" << std::endl;
		return true;
	}
}

void Window::swapBuffers()
{
	if (mainWindow)
		glfwSwapBuffers(mainWindow.get());
	else
		std::cout << "window ptr has been deleted or has not been initialized!\n";
}


bool Window::initialize()
{
	/*
	* initialize GLFW
	* set GL version
	* create window
	* set glFunctionLoader
	* set call back function on windows resize event
	*/
	if (!glfwInit())
	{
		std::cerr << "ERROR::GLFW::Can't initialize GLFW!"<<std::endl;
		return false;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	
	//function to destroy GLFWwindow when smart_ptr runs out of scope
	auto glfwWindowsDestroyer = [](GLFWwindow* ptr) {glfwDestroyWindow(ptr); };
	
	if(share)
		mainWindow.reset(glfwCreateWindow(Width, Height, windowName.c_str(), NULL, share->mainWindow.get()), glfwWindowsDestroyer);
	else
		mainWindow.reset(glfwCreateWindow(Width, Height, windowName.c_str(), NULL, NULL), glfwWindowsDestroyer);
	
	if (mainWindow==NULL)
	{
		std::cerr << "ERROR::GLFW::Couldn't initialize Window!"<<std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(mainWindow.get());


	//load OpenGL function loader into glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr <<"ERROR::GLFW::Could'nt initialize glad!";
		glfwTerminate();
		return false;
	}
	

	
	bufferHeight = Height;
	bufferWidth = Width;
	
	//default call back function when winow is resized
	auto frameBufferSizeCallBack = [](GLFWwindow* window, int width, int height)
	{
		glViewport(0,0,width,height);
	};
	glfwSetFramebufferSizeCallback(mainWindow.get(),frameBufferSizeCallBack);
	glEnable(GL_MULTISAMPLE);
	return true;
}

void Window::setBufferSizeCallBackFunction(void (*function)(GLFWwindow*, int, int))
{
	//set custom function as a call back function of windows resize event
	if (mainWindow)
		glfwSetFramebufferSizeCallback(mainWindow.get(), function);
	else
		std::cerr <<"ERROR::initialize Window first!"<<std::endl;
}





void Window::processInput()
{
	/*processes all inputs given to this window*/
	if (mainWindow)
	{
		if (glfwGetKey(mainWindow.get(), GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(mainWindow.get(), true);
		}
	}
	else
	{
		std::cout <<"Window not intialized!\n";
	}
}