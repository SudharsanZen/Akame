#pragma once


#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<vector>
#include<stdlib.h>
#include<Core/Engine.h>
#include<Rendering/DeferredRendererFragmentBuffer.h>
#include<Core/Scene.h>
#include<Core/Window.h>
#include<sstream>
#include"Core/Editor/EditorUI.h"
#include"FileOpenSaveDialogBox.h"

struct ImGuiIO;
typedef int ImGuiTreeNodeFlags;
class Scene;
class EDFExporter
{
	Scene& m_scene;
	std::shared_ptr<ECS> ecs;
	EDFExporter(Scene& scene, Entity selected);

public:
	void ExportEntity(Entity eid);

};
class Exporter
{

private:
	std::string exportPath;
	std::string importPath;
	std::string iniName;
	//float lx=-125, ly=45, lz=0;
	float lx = -90, ly = 0, lz = 0;

	bool drawLights = false;

	ImGuiIO& io;

	Scene& m_Scene;
	//initializes imGui
	void initImGui();
	FrameBuffer fbo[5];
	//returns the &io value
	ImGuiIO& initGui(Scene& scene);
	float deltaTime, currTime, lastTime;
	std::shared_ptr<SceneHierarchyWindow> m_SceneHierarchy;
	std::shared_ptr<InspectorWindow> m_InspectorWindow;
	std::shared_ptr<FileOpenSaveDialogBox> m_FOSDB;
	std::shared_ptr<ViewPortWindow> m_ViewPortWindow;
public:

	Exporter(Scene& scene);
	//draw UI onto the screen
	void DrawUI();//call after redering scene objects else the UI will be put behind other renderings


};

