#include "ExporterUI.h"
#ifdef _MSC_VER
#define NOMINMAX
#include <windows.h>
#endif // _MSC_VER

#include<locale>
#include<codecvt>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

#include"Core/Editor/PropertiesWindow/InspectorWindow.h"
#include"Core/Editor/SceneHierarchyWindow/SceneHierarchyWindow.h"
#include"Rendering/System/RenderingSystem.h"
#include"Rendering/System/LightSystem.h"
#include"Core/Editor/ViewPort/ViewPortWindow.h"
#define EXPOSE_OS_INTERFACE
#include"Core/OS/OS_Interface.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<GLFW/glfw3native.h>
#pragma warning(push, 0)
#pragma warning(disable : 26495)
#pragma warning(disable : 6031)
#include<imGui\backends\imgui_impl_glfw.h>
#include<imGui\backends\imgui_impl_opengl3.h>
#include<misc/cpp/imgui_stdlib.h>
#include<imgui_internal.h>
#pragma warning(pop)

#include"Core/Serialization/ModelExporter.h"

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
	
	ImGui_ImplGlfw_InitForOpenGL(m_Scene.window.getCurrContext(), true);
	ImGui_ImplOpenGL3_Init("#version 150");
}

ImGuiIO& Exporter::initGui(Scene &scene)
{
	

	glfwMakeContextCurrent(scene.window.getCurrContext());
	//load OpenGL function loader into glad
	if (!gladLoadGLLoader(scene.window.GetProcAddress()))
	{

		glfwTerminate();
		throw("can't do it pal!");
	}
	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& IO = ImGui::GetIO();
	iniName = (AssetManager::assetRootPath + "EngineAssets/Exporter_imgui.ini");
	IO.IniFilename = iniName.c_str();
	ImGui::LoadIniSettingsFromDisk(iniName.c_str());
	return IO;
}

Exporter::Exporter(Scene& scene):m_Scene(scene),io(initGui(scene))
{
	
	initImGui();
	//m_Scene.renderSys->editorMode = true;
	AkameWindowBase wind;
	wind.SetImGuiContext(ImGui::GetCurrentContext());
	m_SceneHierarchy = std::make_shared<SceneHierarchyWindow>(m_Scene);
	
	m_InspectorWindow = std::make_shared<InspectorWindow>(m_Scene,m_Scene.ecs);
	m_FOSDB = std::make_shared<FileOpenSaveDialogBox>("Browse", FileOpenSaveDialogBoxFlag::ak_Open_Mode, AssetManager::assetRootPath);
	
	m_ViewPortWindow = std::make_shared<ViewPortWindow>(m_Scene, io,false);
	m_ViewPortWindow->windowName ="ViewPort";
	
	ImGuiStyle& style = ImGui::GetStyle();
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
					currModel=std::make_shared<Model>(m_Scene);
					currModel->LoadModelToScene(importPath);

				}
				ImGui::SameLine();
				
			}
			if (exportPath != "")
			{
				
					
					Entity selection = m_SceneHierarchy->GetFirstSelection();
				if (selection != INVALID_ENTITY)
				{
					if (ImGui::Button("Export File"))
					{
						ModelExporter exp(m_Scene);
						size_t lastSlash = exportPath.find_last_of("\\");
						size_t dotPose = exportPath.find_last_of(".");
						if (lastSlash == std::string::npos || dotPose == std::string::npos)
						{
							std::cout << "improper path!:" << "Line:" << __LINE__ << " FILE:" << __FILE__;
							return;
						}

						std::string directory = exportPath.substr(0, lastSlash + 1);
						std::string fileName = exportPath.substr(lastSlash + 1, dotPose - lastSlash - 1);
						exp.ExportEntity(selection, directory, fileName);
						exp.ExportAnimationClips(*currModel);
						
					}
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
		glfwGetFramebufferSize(m_Scene.window.getCurrContext(), &display_w, &display_h);
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
		GLFWwindow* backup_current_context = m_Scene.window.getCurrContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	deltaTime = static_cast<float>(glfwGetTime())- lastTime;
	lastTime = static_cast<float>(glfwGetTime());
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
