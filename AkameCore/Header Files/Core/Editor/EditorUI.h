#pragma once


#include"Rendering/Texture.h"
#include"Rendering/FrameBuffer.h"
#include"Assets/ShaderManager.h"
#include"Core\Scene.h"
#include"Core\Window.h"
#include"Core/Editor/ContentBrowsing/ContentBrowser.h"
struct ImGuiIO;
typedef int ImGuiTreeNodeFlags;
class Scene;
struct EntitySignatures
{
private:
	Scene& m_Scene;
public:
	EntitySignatures(Scene& scene);
	ComponentBitPosition m_Transform_pose;
	ComponentBitPosition m_Descriptor_pose;
	ComponentBitPosition m_Mesh_pose;
	ComponentBitPosition m_RigidBody3D_pose;
	ComponentBitPosition m_SkeletalMesh_pose;
	ComponentBitPosition m_AnimationController_pose;
	ComponentBitPosition m_BehaviourComp_pose;
	ComponentBitPosition m_Lights_pose;
	ComponentBitPosition m_Material_pose;
};
class Editor
{

private:
	std::string iniName;
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
	std::shared_ptr<LightAndShadowConfig> m_LightsAndShadows;
	std::shared_ptr<ContentBrowser> m_contentBrowser;
	static void defaultStyle(ImGuiIO& io);
	void Menu();
	void createNewScriptProject();
	bool saveEvent;

	friend class Exporter;
public:
	
	Editor(Scene& m_Scene);
	//draw UI onto the screen
	void DrawUI();//call after redering scene objects else the UI will be put behind other renderings
	~Editor();

	
};

