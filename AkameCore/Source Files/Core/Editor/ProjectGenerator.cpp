#include "..\..\..\Header Files\Core\Editor\ProjectGenerator.h"

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
void ProjectGenerator::show_window() { m_show_window = true; }
void ProjectGenerator::hide_window() { m_show_window = false; }
ProjectGenerator::ProjectGenerator(Scene& scene):m_scene(scene)
{
	akame_root = AssetManager::assetRootPath+"../";
	std::filesystem::path root_path = akame_root;
	akame_root = std::filesystem::absolute(root_path).string();
	solution_root = akame_root+"Assets\\Scripts";
	m_show_window = true;
}

void ProjectGenerator::Draw()
{
	if(m_show_window)
	if (ImGui::Begin("ProjectGenerator",&m_show_window))
	{

		ImGui::Text("Akame Root Directory:");
		ImGui::Separator();
		ImGui::InputText("##AkameRootPath:",&akame_root);
		ImGui::SameLine();

		if (ImGui::Button("...##AkameRootPathButton"))
		{

			akame_root = SelectFolderDialog(akame_root.c_str());
			OpenInFileBrowser(akame_root.c_str());
			ENGINE_CORE_INFO("Selected Akame Root Folder:"+akame_root);
		}

		ImGui::Text("Visual Studio workspace location:");
		ImGui::Separator();
		ImGui::InputText("##VSDir:", &solution_root);
		ImGui::SameLine();

		if (ImGui::Button("...##solution_root"))
		{

			solution_root = SelectFolderDialog(solution_root.c_str());
			
			ENGINE_CORE_INFO("Selected Akame Root Folder:" + solution_root);
		}

		ImGui::Text("VS workspace/solution name:");
		ImGui::Separator();
		ImGui::InputText("##VSsolname:", &solution_name);

		if (ImGui::Button("Generate VS2019 project"))
		{
			std::string cmd = akame_root+"\\Assets\\Scripts\\premake5 --file=\""+akame_root+"Assets\\Scripts\\ScriptGenerator.lua\" vs2019" + " --solution_name=\"" + solution_name + "\" --wks_location=\"" + solution_root + "\\" + solution_name + "\"";
			ENGINE_CORE_INFO(cmd);
			system(cmd.c_str());
			int msgboxID = MessageBox(
				NULL,
				(LPCWSTR)L"Premake Command Executed, check command line for more info!",
				(LPCWSTR)L"Project Generator",
				MB_ICONASTERISK
			);
			OpenInFileBrowser((solution_root+"\\"+solution_name).c_str());
		}
	}
	ImGui::End();
}
