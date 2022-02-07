#pragma once

#include<string>
#include<memory>
#include"AkameCore.h"
typedef struct GLFWwindow GLFWwindow;
class AKAME_API Window
{
protected:
	
private:
	
	//window intialization height and width
	int Height, Width;
	//render buffer dimensions. (usually updated every time using a callback function when the windows reasize event occurs)
	int bufferHeight, bufferWidth;

	std::string windowName;
	//window class to share opengl context with
	Window* share;
	//main GLFWwindow pointer.(generated by a call to create window)
	std::shared_ptr<GLFWwindow> mainWindow;

	
public:
	Window(int height,int width,std::string,Window* shr=nullptr);
	~Window();
	friend class Editor;
	friend class Input;
	friend class Exporter;

	int getBufferHeight();
	int getBufferWidth();

	bool Window::closeWindow();
	void swapBuffers();

	bool initialize();
	void setBufferSizeCallBackFunction(void (*function)(GLFWwindow*, int, int) );
	void processInput();
	
};

