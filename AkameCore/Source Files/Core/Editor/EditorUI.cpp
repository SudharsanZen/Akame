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

#include<imGui\backends\imgui_impl_glfw.h>
#include<imGui\backends\imgui_impl_opengl3.h>
float pssmx = 1.45f;
float pssmy = 1.45f;
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
	return ImGui::GetIO();
}

void Editor::defaultStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	io.Fonts->AddFontFromFileTTF((AssetManager::assetRootPath+"EngineAssets/liberation-sans/LiberationSans-Regular.ttf").c_str(),15.0f);
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

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.91f, 0.91f, 0.91f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.07f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.61f, 0.61f, 0.61f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.62f, 0.62f, 0.62f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.81f, 0.81f, 0.81f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.47f, 0.47f, 0.47f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.56f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.48f, 0.48f, 0.48f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.29f, 0.29f, 0.29f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.68f, 0.66f, 0.66f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.64f, 0.64f, 0.64f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.66f, 0.66f, 0.66f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	

}

Editor::Editor(Scene &m_Scene):io(initGui()),m_Scene(m_Scene)
{
	std::weak_ptr<GLFWwindow> context = m_Scene.window.mainWindow;
	m_Scene.renderSys->editorMode = true;
	initImGui();
	//defaultStyle();
	m_SceneHierarchy = std::make_shared<SceneHierarchyWindow>(m_Scene);
	m_InspectorWindow= std::make_shared<InspectorWindow>(m_Scene,m_Scene.ecs);
	m_ViewPortWindow.push_back(std::make_shared<ViewPortWindow>(m_Scene,io));
	m_ViewPortWindow[0]->windowName = "Scene##0";
	m_ViewPortWindow.push_back(std::make_shared<ViewPortWindow>(m_Scene,io));
	m_ViewPortWindow[1]->windowName = "Scene##1";

	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
}


void Editor::DrawUI()
{
	std::shared_ptr<GLFWwindow> context = m_Scene.window.mainWindow;
	
	currTime = glfwGetTime();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();

	
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Windows##1"))
	{
		if (ImGui::BeginMenu("new"))
		{
			if (ImGui::MenuItem("ViewPort"))
			{
				std::shared_ptr<ViewPortWindow> vp = std::make_shared<ViewPortWindow>(m_Scene,io);
				std::stringstream ss;
				ss << m_ViewPortWindow.size();
				vp->windowName = "Scene##"+ss.str();
				m_ViewPortWindow.push_back(vp);
			}
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	ImGui::DockSpaceOverViewport();

	
	
	m_SceneHierarchy->Draw();
	m_InspectorWindow->Draw(m_SceneHierarchy);
	//draw viewPortWindow-------

		m_Scene.EDS->updateMap();
		m_Scene.behaviourSys->Update(deltaTime);
		m_Scene.animSys->Run();
		m_Scene.animContSys->update(deltaTime);
		m_Scene.transformManager->UpdateTransforms();
		Debug::updateBufferContent();
		bool viewPortListUpdate;
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
	ImGui::Begin("DirShadowMaps");
	//get the mouse position
	ImGui::SliderFloat("multX", &pssmx, 1.0f, 2.0f, "%.3f");
	ImGui::SliderFloat("multY", &pssmy, 1.0f, 2.0f, "%.3f");
	static float maxz = 1000;
	static float minz = 100;
	ImGui::DragScalar("ShadowDist", ImGuiDataType_Float, &m_Scene.lightSys->shadowDistance, 1,&minz,&maxz, "%f");
	bool clicked=ImGui::Button("Reload Shaders",ImVec2(200.0f,25.0f) );
	
	if (clicked)
	{

		ShaderManager::releaseAllShaders();
		ShaderManager::loadAllShaders();
		m_Scene.renderSys->attachAllBuiltInSRP();
		
	}
	ImGui::SliderFloat("log-uniform lambda", &m_Scene.lightSys->lambda, 0.0f, 1.0f, "ratio = %.3f");
	ImVec2 pos = ImGui::GetCursorScreenPos();
	
	//pass the texture of the FBO
	//window.getRenderTexture() is the texture of the FBO
	//the next parameter is the upper left corner for the uvs to be applied at
	//the third parameter is the lower right corner
	//the last two parameters are the UVs
	//they have to be flipped (normally they would be (0,0);(1,1) 
	
	
	ShaderManager::GetShader("PSSM")->useShaderProgram();
	std::vector<unsigned int> texList;
	for (int i = 0; i < m_Scene.lightSys->dirLightSpace.size(); i++)
	{
		glDisable(GL_CULL_FACE);
		fbo[i].Bind();
		ShaderManager::GetShader("PSSM")->setUniformInteger("layer", i);
		m_Scene.lightSys->dir_sMap.useTextureArray(0);
		glBindVertexArray(Mesh::VAO);
		fbo[i].quadMesh.renderMesh();
		glBindVertexArray(0);
		fbo[i].unBind();
		glEnable(GL_CULL_FACE);
		texList.push_back(fbo[i].getColorBuffer());
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
	lastTime = currTime;
}

