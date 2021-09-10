
#include"Core/Window.h"
#include<GLFW/glfw3.h>
#include<memory>
#include<iostream>
#include "Core/Input.h"


Input::keyState Input::keyPressed[1024];


void Input::flush()
{
	for (int i = 0; i < 1024; i++)
	{
		keyPressed[i].up = false;
		keyPressed[i].down = false;
		
	}
	
}
bool Input::getKeyDown(int key)
{
	return (keyPressed[key].down);
}

bool Input::getKeyUp(int key)
{
	return (keyPressed[key].up);

}
void Input::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{

	if (key >= 0 && key < 1024)
	{
		float currKeyState = Input::keyPressed[key].currentState;
		if (action == GLFW_PRESS || action == GLFW_RELEASE)
		{
			Input::keyPressed[key].oldState = currKeyState;
			Input::keyPressed[key].currentState = action;
			if (currKeyState == GLFW_PRESS && action == GLFW_RELEASE)
			{
				//std::cout << "Up";
				Input::keyPressed[key].up = true;
			}
			else if (currKeyState == GLFW_RELEASE && action == GLFW_PRESS)
			{
				//std::cout << "Down";
				Input::keyPressed[key].down = true;
			}

		}

	}
}
bool Input::getKey(int key)
{
	/**/static GLFWwindow* ptr = glfwGetCurrentContext();

	if (ptr)
	{
		return (glfwGetKey(ptr, key) == GLFW_PRESS);
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

