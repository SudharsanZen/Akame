#include "Core\Editor\EditorUI.h"
#include<iostream>
#include"Core/OS/OS_Interface.h"
#include"Core\Input.h"
#include<sstream>
#include"Components/EntityDescriptor.h"
#include"Rendering/System/LightSystem.h"
#include"Rendering/System/RenderingSystem.h"
#include"Rendering/System/DeferredPipeline.h"
#include"Rendering/System/PSSMFBO.h"
#include"Core/Debug/Debug.h"
#include"Animation/AnimationControllerSystem.h"
#include"Core/Editor/PropertiesWindow/InspectorWindow.h"
#include"Core/Editor/SceneHierarchyWindow/SceneHierarchyWindow.h"
#include"Core/Editor/ViewPort/ViewPortWindow.h"
#include"Core/Editor/SettingsWindow/LightsAndShadows.h"
#include"Assets/Asset.h"
#include"ECS.h"
#include"Core/Serialization/SceneSerialization.h"
#pragma warning(push, 0)
#pragma warning( disable : 26495)
#pragma warning( disable : 6031)
#pragma warning( disable : 26812)
#include<imGui\backends\imgui_impl_glfw.h>
#include<imGui\backends\imgui_impl_opengl3.h>
#include<glad/glad.h>
#include<glfw/glfw3.h>
#pragma warning(pop)
void Editor::initImGui()
{
	std::weak_ptr<GLFWwindow> context=m_scene.window.mainWindow;
	
	fbo[0].updateTextureSize(512,512);
	fbo[1].updateTextureSize(512,512);
	fbo[2].updateTextureSize(512,512);
	fbo[3].updateTextureSize(512,512);
	fbo[4].updateTextureSize(512,512);
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;


	// Setup Dear ImGui style
	ImGui::StyleColorsDark();


	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(context.lock().get(), true);
	ImGui_ImplOpenGL3_Init("#version 150");
}

ImGuiIO& Editor::initGui()
{

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& IO = ImGui::GetIO();
	iniName = (AssetManager::assetRootPath + "EngineAssets/imgui.ini");
	IO.IniFilename = iniName.c_str();
	ImGui::LoadIniSettingsFromDisk(iniName.c_str());
	return IO ;
}

void Editor::defaultStyle(ImGuiIO& io)
{
	ImGuiStyle& style = ImGui::GetStyle();
	io.Fonts->AddFontFromFileTTF((AssetManager::assetRootPath+"EngineAssets/liberation-sans/LiberationSans-Regular.ttf").c_str(),15.0f);
	

	//imGuiIO.Fonts->AddFontFromFileTTF("../data/Fonts/Ruda-Bold.ttf", 15.0f, &config);
	ImGui::GetStyle().FrameRounding = 4.0f;
	ImGui::GetStyle().GrabRounding = 4.0f;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	
	
	colors[ImGuiCol_Header] = ImVec4(1.00f, 0.34f, 0.34f, 0.55f);


}

void Editor::Menu()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Menu##EDITOR"))
	{
		if (ImGui::BeginMenu("New"))
		{
			ImGui::MenuItem("Project");
			ImGui::MenuItem("Scene");
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Open"))
		{
			ImGui::MenuItem("Project");
			if (ImGui::MenuItem("Scene"))
			{
				SceneDeserializer des(m_scene);
				des.Load(true);
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Import File"))
		{
			SceneDeserializer des(m_scene);
			const char* filter_edf = "Akame Entity Definition(*edf)\0 * edf\0";
			std::string path = OpenFileDialog(filter_edf,"edf");
			if(path!="")
				des.LoadFrom(path);
		}
		ImGui::Separator();
		ImGui::MenuItem("save");
		if (ImGui::MenuItem("save as"))
		{
			SceneSerializer ser(m_scene);
			ser.Save();
		}
		ImGui::Separator();
		if (ImGui::BeginMenu("Recent Files"))
		{
			ImGui::MenuItem("Bababoye");
			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Edit##Editor"))
	{
		if (ImGui::BeginMenu("Scriptable Project"))
		{
			if (ImGui::MenuItem("Create New"))
				m_proj_gen.show_window();
			if (ImGui::MenuItem("Set Current"))
			{
				std::string init_dir = AssetManager::assetRootPath + "../";
				init_dir += "Assets\\Scripts";
				std::string proj_dir=SelectFolderDialog(init_dir.c_str());
				m_scene.scriptSys->set_watch_directory(proj_dir);

			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Windows##EDITOR"))
	{
		if (ImGui::BeginMenu("new"))
		{
			if (ImGui::MenuItem("ViewPort"))
			{
				std::shared_ptr<ViewPortWindow> vp = std::make_shared<ViewPortWindow>(m_scene, io);
				std::stringstream ss;
				ss << m_ViewPortWindow.size();
				vp->windowName = "Scene##" + ss.str();
				m_ViewPortWindow.push_back(vp);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();


}

void Editor::createNewScriptProject()
{

}

Editor::Editor(Scene &m_scene):io(initGui()),m_scene(m_scene),m_proj_gen(m_scene)
{
	
	std::weak_ptr<GLFWwindow> context = m_scene.window.mainWindow;
	m_scene.renderSys->editorMode = true;
	initImGui();
	defaultStyle(io);
	m_SceneHierarchy = std::make_shared<SceneHierarchyWindow>(m_scene);
	m_InspectorWindow= std::make_shared<InspectorWindow>(m_scene,m_scene.ecs);
	m_LightsAndShadows = std::make_shared<LightAndShadowConfig>(m_scene);
	m_ViewPortWindow.push_back(std::make_shared<ViewPortWindow>(m_scene,io));
	m_ViewPortWindow[0]->windowName = "Scene##0";
	m_ViewPortWindow.push_back(std::make_shared<ViewPortWindow>(m_scene,io));
	m_ViewPortWindow[1]->windowName = "Scene##1";
	m_proj_gen.hide_window();
	deltaTime	= 0.0f;
	lastTime	= 0.0f;
	currTime	= 0.0f;
	m_contentBrowser = std::make_shared<ContentBrowser>("Project",AssetManager::assetRootPath);
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
}


void Editor::DrawUI()
{
	std::shared_ptr<GLFWwindow> context = m_scene.window.mainWindow;
	
	currTime = static_cast<float>(glfwGetTime());
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();

	
	Menu();
	ImGui::DockSpaceOverViewport();
	m_proj_gen.Draw();
	ImGui::ShowDemoWindow();
	m_LightsAndShadows->Draw();
	m_SceneHierarchy->Draw();
	m_contentBrowser->DrawUI();
	m_InspectorWindow->Draw(m_SceneHierarchy);
	//draw viewPortWindow-------

		m_scene.EDS->updateMap();
		m_scene.lightSys->Update();
		m_scene.behaviourSys->Update(deltaTime);
		m_scene.scriptSys->Update(deltaTime);
		m_scene.animSys->Run();
		m_scene.animContSys->update(deltaTime);
		m_scene.transformManager->UpdateTransforms();
		
		Debug::updateBufferContent();
		bool viewPortListUpdate=false;
		for (auto viewPort : m_ViewPortWindow)
		{
			viewPortListUpdate |= !viewPort->open;
			viewPort->Draw(deltaTime);
		}
		if (viewPortListUpdate)
		{
			std::vector<std::shared_ptr<ViewPortWindow>> newViewPortList;
			for (auto viewPort: m_ViewPortWindow)
			{
				if(viewPort->open)
					newViewPortList.push_back(viewPort);
			}
			m_ViewPortWindow = newViewPortList;
		}
		Debug::FlushDebugInformation();
		m_scene.physicsSys->Run(deltaTime);

	//---------------------------
	/*
	bool clicked = ImGui::Button("Reload Shaders", ImVec2(200.0f, 25.0f));

	if (clicked)
	{

		ShaderManager::releaseAllShaders();
		ShaderManager::loadAllShaders();
		m_scene.renderSys->attachAllBuiltInSRP();

	}
	std::shared_ptr<DeferredPipeline> dfrp = std::static_pointer_cast<DeferredPipeline>(ShaderManager::shaderRenderPipeline["DEFERRED"]);
	if (dfrp)
	{
		texList.push_back(dfrp->drfb.drfb.getAlbedoBuff());
		texList.push_back(dfrp->drfb.drfb.getPositionBuff());
		texList.push_back(dfrp->drfb.drfb.Normal);
		texList.push_back(dfrp->drfb.drfb.getDepthBuff());
	}
	
	int div = 2,res=512/3;
	for (int i = 0; i < texList.size(); i++)
	{
		
		float size = res;
		float gapx = 5.0f*(i%div);
		float gapy = 5.0f*((i/div));
		float dispY = (i / div) * (size);
		ImVec2 pos= ImGui::GetCursorScreenPos();
		float x = pos.x + (size)*((i)%div);
		float y = pos.y + dispY;
		unsigned int tex = texList[i];
		ImGui::GetWindowDrawList()->AddImage(
			(void*)tex,
			ImVec2(ImVec2(x+gapx,y+gapy)),
			ImVec2(x+size+gapx ,
				y+size+gapy), ImVec2(0, 1), ImVec2(1, 0));
	}
	ImGui::End();
	*/
		if (ImGui::Begin("stats"))
		{
			ImGui::Text("Time:%f ms", deltaTime * 1000.0f);
			ImGui::Text("fps:%d", static_cast<int>(1.0f / (deltaTime+0.0000001f)));
			ImGui::Text("Time Culling:%f ms",m_scene.renderSys->time_taken);
			

		}
		ImGui::End();
	ImGui::Render();
	int display_w=0, display_h=0;
	std::shared_ptr<GLFWwindow> windowContext = context;
	if (windowContext)
	{
		glfwGetFramebufferSize(windowContext.get(), &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
	}
	else
	{
		std::cout << "window context not initialized (or) is destroyed!\n";
		return;
	}

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	deltaTime = currTime - lastTime;
	m_scene.deltaTime = deltaTime;
	lastTime = currTime;
	if (io.KeysDown[KEY_LEFT_CONTROL])
	{
		if (io.KeysDown[KEY_S] && !saveEvent)
		{
			saveEvent = true;
		}
	}

	if (saveEvent)
	{
		saveEvent = false;
		
	}
}

Editor::~Editor()
{
	ImGui::SaveIniSettingsToDisk(iniName.c_str());
}

EntitySignatures::EntitySignatures(Scene& scene) :m_scene(scene)
{
	auto m_ECS=m_scene.ecs;

	m_Transform_pose = m_ECS->GetComponentBitPose<Transform>();
	m_Descriptor_pose = m_ECS->GetComponentBitPose<EntityDescriptor>();
	m_Mesh_pose = m_ECS->GetComponentBitPose<Mesh>();
	m_RigidBody3D_pose = m_ECS->GetComponentBitPose<physics::RigidBody3D>();
	m_SkeletalMesh_pose = m_ECS->GetComponentBitPose<SkeletalMesh>();
	m_AnimationController_pose = m_ECS->GetComponentBitPose<AnimationController>();
	m_BehaviourComp_pose = m_ECS->GetComponentBitPose<BehaviourComponent>();
	m_Lights_pose = m_ECS->GetComponentBitPose<Lights>();
	m_Material_pose = m_ECS->GetComponentBitPose<Material>();
}
