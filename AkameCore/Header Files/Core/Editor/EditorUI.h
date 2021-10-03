#pragma once
#include"Core/Window.h"
#include"Core/Scene.h"
#include"Rendering/Texture.h"
#include"Rendering/FrameBuffer.h"
#include"Assets/ShaderManager.h"
struct ImGuiIO;
typedef int ImGuiTreeNodeFlags;
class Editor
{
private:
	float lx=-90, ly=0, lz=0;
	bool viewDebugInfo;
	std::set<Entity> selected;
	ImGuiIO& io;
	std::weak_ptr<GLFWwindow> context;
	Scene &scene;
	//initializes imGui
	void initImGui();
	FrameBuffer fbo[5];
	//returns the &io value
	ImGuiIO& initGui();
	void DrawNode(Transform const& t, EntityDescriptor& edt, ImGuiTreeNodeFlags const& base_flags);
public:
	
	Editor(Window &window,Scene& scene);
	//draw UI onto the screen
	void DrawUI();//call after redering scene objects else the UI will be put behind other renderings


	
};

