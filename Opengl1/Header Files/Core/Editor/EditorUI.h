#pragma once
#include"Core/Window.h"
#include"Core/Scene.h"
struct ImGuiIO;
typedef int ImGuiTreeNodeFlags;
class Editor
{
private:
	std::set<EntityID> selected;
	ImGuiIO& io;
	std::weak_ptr<GLFWwindow> context;
	Scene &scene;
	//initializes imGui
	void initImGui();
	//returns the &io value
	ImGuiIO& initGui();

public:
	
	Editor(Window &window,Scene& scene);
	//draw UI onto the screen
	void DrawUI();//call after redering scene objects else the UI will be put behind other renderings


	void DrawNode(Transform const& t, EntityDescriptor  &edt, ImGuiTreeNodeFlags const& base_flags);
};

