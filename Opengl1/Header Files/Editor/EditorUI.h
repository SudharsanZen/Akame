#pragma once
#include"Window.h"

struct ImGuiIO;

class Editor
{
private:
	ImGuiIO& io;
	std::weak_ptr<GLFWwindow> context;

	//initializes imGui
	void initImGui();
	//returns the &io value
	ImGuiIO& initGui();

public:
	
	Editor(Window &window);
	//draw UI onto the screen
	void DrawUI();//call after redering scene objects else the UI will be put behind other renderings
};

