#include<iostream>
#include "Core\Editor\EditorUI.h"
#include"glad/glad.h"
#include"glfw/glfw3.h"
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
#include<imGui\backends\imgui_impl_glfw.h>
#include<imGui\backends\imgui_impl_opengl3.h>

void Editor::initImGui()
{
	std::weak_ptr<GLFWwindow> context=m_Scene.window.mainWindow;
	
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

void Editor::defaultStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	io.Fonts->AddFontFromFileTTF((AssetManager::assetRootPath+"EngineAssets/liberation-sans/LiberationSans-Regular.ttf").c_str(),15.0f);
	/*
	//padding
	style.WindowPadding = ImVec2(11.0f,10.0f);
	style.FramePadding = ImVec2(4.0f,4.0f);
	style.CellPadding = ImVec2(0.0f,1.0f);
	style.ItemSpacing = ImVec2(8.0f,4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f,4.0f);
	style.IndentSpacing = 6.0f;
	style.ScrollbarSize = 15.0f;
	style.GrabMinSize = 15.0f;
	//borders
	style.WindowBorderSize = 1.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;
	//rounding
	style.FrameRounding = 2.0f;
	style.WindowRounding = 2.0f;
	style.ChildRounding = 1.0f;
	style.PopupRounding = 2.0f;
	style.ScrollbarRounding = 2.0f;
	style.GrabRounding = 2.0f;
	style.LogSliderDeadzone = 4.0f;
	style.TabRounding = 4.0f;
	//alignment
	style.WindowTitleAlign = ImVec2(0.5f,0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_None;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	*/
	// Sorry if there's any errors here, I translated this back by hand.
	
	

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
			ImGui::MenuItem("Level");
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Open"))
		{
			ImGui::MenuItem("Project");
			ImGui::MenuItem("Level");
			ImGui::EndMenu();
		}
		ImGui::Separator();
		ImGui::MenuItem("save");
		ImGui::MenuItem("save as");
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
			ImGui::MenuItem("Create New");
			ImGui::MenuItem("Set Current");
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
				std::shared_ptr<ViewPortWindow> vp = std::make_shared<ViewPortWindow>(m_Scene, io);
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

Editor::Editor(Scene &m_Scene):io(initGui()),m_Scene(m_Scene)
{
	
	std::weak_ptr<GLFWwindow> context = m_Scene.window.mainWindow;
	m_Scene.renderSys->editorMode = true;
	initImGui();
	defaultStyle();
	m_SceneHierarchy = std::make_shared<SceneHierarchyWindow>(m_Scene);
	m_InspectorWindow= std::make_shared<InspectorWindow>(m_Scene,m_Scene.ecs);
	m_LightsAndShadows = std::make_shared<LightAndShadowConfig>(m_Scene);
	m_ViewPortWindow.push_back(std::make_shared<ViewPortWindow>(m_Scene,io));
	m_ViewPortWindow[0]->windowName = "Scene##0";
	m_ViewPortWindow.push_back(std::make_shared<ViewPortWindow>(m_Scene,io));
	m_ViewPortWindow[1]->windowName = "Scene##1";
	ContentBrowserFlags flg = ak_Open_Mode | ak_Only_Show_given_files;
	m_contentBrowser = std::make_shared<ContentBrowser>("Project",flg,AssetManager::assetRootPath);
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
}


void Editor::DrawUI()
{
	std::shared_ptr<GLFWwindow> context = m_Scene.window.mainWindow;
	
	currTime = glfwGetTime();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();

	
	Menu();
	ImGui::DockSpaceOverViewport();

	ImGui::ShowDemoWindow();
	m_LightsAndShadows->Draw();
	m_SceneHierarchy->Draw();
	m_contentBrowser->DrawUI();
	m_InspectorWindow->Draw(m_SceneHierarchy);
	//draw viewPortWindow-------

		m_Scene.EDS->updateMap();
		m_Scene.lightSys->Update();
		m_Scene.behaviourSys->Update(deltaTime);
		m_Scene.animSys->Run();
		m_Scene.animContSys->update(deltaTime);
		m_Scene.transformManager->UpdateTransforms();
		
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
		m_Scene.physicsSys->Run(deltaTime);

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
	m_Scene.deltaTime = deltaTime;
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

