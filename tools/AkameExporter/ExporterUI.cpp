#include "ExporterUI.h"
#include "glad/glad.h"
#include<GLFW/glfw3.h>
#include<locale>
#include<codecvt>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include<GLFW/glfw3native.h>
#include<Windows.h>
#include"Core/Editor/PropertiesWindow/InspectorWindow.h"
#include<imGui\backends\imgui_impl_glfw.h>
#include<imGui\backends\imgui_impl_opengl3.h>
#include<misc/cpp/imgui_stdlib.h>
#include"Core/Editor/SceneHierarchyWindow/SceneHierarchyWindow.h"
#include<Windows.h>
#include"Rendering/System/RenderingSystem.h"
#include"Rendering/System/LightSystem.h"
#include"Core/Editor/ViewPort/ViewPortWindow.h"
#define EXPOSE_OS_INTERFACE
#include"OS_interface.h"
void Exporter::initImGui()
{
	std::weak_ptr<GLFWwindow> context = m_Scene.window.mainWindow;


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

ImGuiIO& Exporter::initGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& IO = ImGui::GetIO();
	iniName = (AssetManager::assetRootPath + "EngineAssets/Exporter_imgui.ini");
	IO.IniFilename = iniName.c_str();
	ImGui::LoadIniSettingsFromDisk(iniName.c_str());
	return IO;
}

Exporter::Exporter(Scene& scene):m_Scene(scene),io(initGui())
{
	initImGui();
	//m_Scene.renderSys->editorMode = true;
	m_SceneHierarchy = std::make_shared<SceneHierarchyWindow>(m_Scene);
	m_InspectorWindow = std::make_shared<InspectorWindow>(m_Scene,m_Scene.ecs);
	m_FOSDB = std::make_shared<FileOpenSaveDialogBox>("Browse", ak_Open_Mode, AssetManager::assetRootPath);
	m_ViewPortWindow = std::make_shared<ViewPortWindow>(m_Scene, io,false);
	m_ViewPortWindow->windowName ="ViewPort";
	Editor::defaultStyle(io);
	m_FOSDB->reset();
}

void Exporter::DrawUI()
{
	static float deltaTime = 0;
	static float lastTime = 0;

	constexpr char* filter = "Entity definition(*edf)\0*edf\0";
	constexpr char* ifilter = "(*fbx)\0*fbx\0(*obj)\0*obj\0(*.*)\0*.*\0";
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport();
	static bool importDiag=false;
	static bool exportDiag=false;
	{
		if (ImGui::Begin("Exporter"))
		{
			ImGui::Text("Destination Directory");
			ImGui::Separator();
			ImGui::InputText("##Export Path:", &exportPath);

			ImGui::SameLine();

			if (ImGui::Button("...##ExportPathButton"))
			{

				exportPath = SaveFileDialog(filter, "edf");
			}
			ImGui::Text("Source File");
			ImGui::Separator();
			ImGui::InputText("##Import Path:", &importPath);

			ImGui::SameLine();

			if (ImGui::Button("...##ImportPathButton"))
			{
				importPath = OpenFileDialog(ifilter, importPath.c_str());
			}

			if (importPath != "")
			{
				if (ImGui::Button("Import File"))
				{
					Model modelLoader(m_Scene);
					modelLoader.LoadModelToScene(importPath);

				}
			}
			ImGui::End();
		}
	}
	
	m_SceneHierarchy->Draw();
	m_InspectorWindow->Draw(m_SceneHierarchy);

	m_Scene.EDS->updateMap();
	m_Scene.lightSys->Update();
	m_Scene.behaviourSys->Update(deltaTime);
	m_Scene.animSys->Run();
	//m_Scene.animContSys->update(deltaTime);
	m_Scene.transformManager->UpdateTransforms();
	m_ViewPortWindow->open = true;
	m_ViewPortWindow->Draw(deltaTime);

	ImGui::Render();
	int display_w = 0, display_h = 0;
	
	if (m_Scene.window.mainWindow)
	{
		glfwGetFramebufferSize(m_Scene.window.mainWindow.get(), &display_w, &display_h);
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

	deltaTime = glfwGetTime()- lastTime;
	lastTime = glfwGetTime();
	m_Scene.deltaTime = deltaTime;
}

EDFExporter::EDFExporter(Scene& scene, Entity selected) :m_scene(scene)
{
	ecs = m_scene.ecs;
}

void EDFExporter::ExportEntity(Entity eid)
{
	if (eid == INVALID_ENTITY)
		throw("Invalid entity!");

}
