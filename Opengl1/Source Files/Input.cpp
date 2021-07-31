#include "Input.h"
#include"Window.h"
#include<GLFW/glfw3.h>
#include<memory>
#include<iostream>



bool Input::getKeyDown(int key)
{
	static GLFWwindow* ptr = glfwGetCurrentContext();

	if(ptr)
	{
		return (glfwGetKey(ptr, key) == GLFW_PRESS);
		
	}
	else
	{
		std::cout << "INPUT::Can't get glfw context!\n";
	}
	return false;
}

bool Input::getKeyUp(int key)
{
	static GLFWwindow* ptr = glfwGetCurrentContext();

	if (ptr)
	{
		return ((ptr, key) == GLFW_RELEASE);
	}
	else
	{
		std::cout << "INPUT::Can't get glfw context!\n";
	}
	return false;
}

bool Input::getKey(int key)
{
	static GLFWwindow* ptr = glfwGetCurrentContext();

	if (ptr)
	{
		return (glfwGetKey(ptr, key) == GLFW_REPEAT);
	}
	else
	{
		std::cout << "INPUT::Can't get glfw context!\n";
	}
	return false;
}

void Input::getMouseXY(double& x, double& y)
{
	GLFWwindow* ptr =glfwGetCurrentContext();
	if (ptr)
	{
		glfwGetCursorPos(ptr,&x,&y);
	}

}

int Input::getMouseButton(int button)
{
	return glfwGetMouseButton(glfwGetCurrentContext(),button);
}

