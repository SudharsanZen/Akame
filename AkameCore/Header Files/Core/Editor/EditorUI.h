#pragma once


#include"Rendering/Texture.h"
#include"Rendering/FrameBuffer.h"
#include"Assets/ShaderManager.h"
#include"Core\Scene.h"
#include"Core\Window.h"
struct ImGuiIO;
typedef int ImGuiTreeNodeFlags;
class Scene;
class Editor
{
private:
	//float lx=-125, ly=45, lz=0;
	float lx=-90, ly=0, lz=0;

	bool drawLights=false;

	ImGuiIO& io;
	
	Scene &m_Scene;
	//initializes imGui
	void initImGui();
	FrameBuffer fbo[5];
	//returns the &io value
	ImGuiIO& initGui();
	float deltaTime,currTime,lastTime;
	std::shared_ptr<SceneHierarchyWindow> m_SceneHierarchy;
	std::shared_ptr<InspectorWindow> m_InspectorWindow;
	std::vector<std::shared_ptr<ViewPortWindow>> m_ViewPortWindow;
	void defaultStyle();
public:
	
	Editor(Scene& m_Scene);
	//draw UI onto the screen
	void DrawUI();//call after redering scene objects else the UI will be put behind other renderings


	
};

