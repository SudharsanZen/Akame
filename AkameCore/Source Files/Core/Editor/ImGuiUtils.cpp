
#include "..\..\..\Header Files\Core\Editor\ImGuiUtils.h"
#pragma warning(push, 0)
#pragma warning( disable : 26495)
#pragma warning( disable : 6031)
#pragma warning( disable : 26812)
#include<imGui\backends\imgui_impl_glfw.h>
#include<imGui\backends\imgui_impl_opengl3.h>
#include<glad/glad.h>
#include<glfw/glfw3.h>
#pragma warning(pop)


void dragFloat3(std::string* names, float* values, float width)
{
    ImGui::PushItemWidth(width);
    ImGui::DragFloat(names[0].c_str(), &values[0]);   ImGui::SameLine();
    ImGui::DragFloat(names[1].c_str(), &values[1]); ImGui::SameLine();
    ImGui::DragFloat(names[2].c_str(), &values[2]);
    ImGui::PopItemWidth();
}

void DrawHeadingWithSeperator(std::string name)
{
    ImGui::Text(name.c_str());
    ImGui::Separator();
}

void dragFloat4(std::string* names, float* values, float width)
{
    ImGui::PushItemWidth(width);
    ImGui::DragFloat(names[0].c_str(), &values[0]);   ImGui::SameLine();
    ImGui::DragFloat(names[1].c_str(), &values[1]);   ImGui::SameLine();
    ImGui::DragFloat(names[2].c_str(), &values[2]);   ImGui::SameLine();
    ImGui::DragFloat(names[3].c_str(), &values[3]);
    ImGui::PopItemWidth();
}

