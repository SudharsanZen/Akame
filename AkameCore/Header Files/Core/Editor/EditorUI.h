#pragma once


#include"Rendering/Texture.h"
#include"Rendering/FrameBuffer.h"
#include"Assets/ShaderManager.h"
#include"Core\Scene.h"
#include"Core\Window.h"
#include"Core/Editor/ContentBrowsing/ContentBrowser.h"
#include"Core/Editor/ProjectGenerator.h"
struct ImGuiIO;
typedef int ImGuiTreeNodeFlags;
class Scene;

struct EntitySignatures
{
private:
	Scene& m_scene;
public:
	AKAME_API EntitySignatures(Scene& scene);
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
	ProjectGenerator m_proj_gen;
	Scene &m_scene;
	//initializes imGui
	AKAME_API void initImGui();
	FrameBuffer fbo[5];
	//returns the &io value
	AKAME_API ImGuiIO& initGui();
	float deltaTime,currTime,lastTime;
	std::shared_ptr<SceneHierarchyWindow> m_SceneHierarchy;
	std::shared_ptr<InspectorWindow> m_InspectorWindow;
	std::vector<std::shared_ptr<ViewPortWindow>> m_ViewPortWindow;
	std::shared_ptr<LightAndShadowConfig> m_LightsAndShadows;
	std::shared_ptr<ContentBrowser> m_contentBrowser;
	AKAME_API static void defaultStyle(ImGuiIO& io);
	AKAME_API void Menu();
	AKAME_API void createNewScriptProject();
	bool saveEvent;

	friend class Exporter;
public:
	
	AKAME_API Editor(Scene& m_scene);
	//draw UI onto the screen
	AKAME_API void DrawUI();//call after redering scene objects else the UI will be put behind other renderings
	AKAME_API ~Editor();

	
};

